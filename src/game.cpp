#include "game.h"
#include "texturemanager.h"
#include "entity.h"
#include "texturemap.h"
#include "player.h"
#include "keyboardcontroller.h"
#include "interactables.h"

std::vector<Window*> Game::windows = {};

int Game::activeWindowIndex = -1;
Window* Game::activeWindow = nullptr;

// Globals (structure to be reorganized so these can be data members)
Player* player = nullptr;
std::vector<TextureMap*> maps = {};
KeyboardController* controller = nullptr;
std::vector<Hazard*> hazards = {};
Goal* goal = nullptr;

Window::Window():
window(nullptr), renderer(nullptr)
{
    rect.x = 0;
    rect.y = 0;
    rect.h = 0;
    rect.w = 0;
}

Window::Window(const Window& other) {
    std::cout << "window copy created";
}

Game::Game() {

}

Game::~Game() {
    std::cout << "Game destroyed" << std::endl;
}

bool Game::SDL_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        std::cout << "SDL_Init failed with error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height) {
    if(!SDL_init()) {
        cleanup();
        isRunning = false;
        return false; 
    }
    std::cout << "SDL initialized" << std::endl;

    if (!loadLevel("levels/newlevel.txt")) {
        cleanup();
        isRunning = false;
        return false;
    }
    if (startingPosition.size() != 3) {
        std::cout << "Could not read level" << std::endl;
        cleanup();
        isRunning = false;
        return false;
    }
    player = new Player(startingPosition[1], startingPosition[2]);
    controller = new KeyboardController();
    return true;
}

bool Game::newWindow(const char* title, int xpos, int ypos, int width, int height) {
    Window* newWindow = new Window();
    newWindow->window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_OPENGL);
    if (newWindow->window == nullptr) {
        std::cout << "unable to create secondary window with error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        isRunning = false;
        return false;
    }
    std::cout << "Window created" << std::endl;
    newWindow->renderer = SDL_CreateRenderer(newWindow->window, -1, SDL_RENDERER_PRESENTVSYNC);
    std::cout << "Renderer created" << std::endl;
    newWindow->rect.x = xpos;
    newWindow->rect.y = ypos;
    newWindow->rect.w = width;
    newWindow->rect.h = height;
    isRunning = true;
    Game::windows.push_back(newWindow);
    return true;
}

bool Game::loadLevel(const char* filename) {
    closeAll();
    std::ifstream ifile(filename);
    if (ifile.fail()) {
        std::cout << "could not open file" << std::endl;
        return false;
    }
    int x, y, w, h, x0, y0, w0, numWindows, numColliders, numHazards;
    std::vector<int> goalPosition;

    ifile >> numWindows;
    ifile >> w0 >> x0 >> y0;
    startingPosition.push_back(w0);
    startingPosition.push_back(x0);
    startingPosition.push_back(y0);
    ifile >> w0 >> x0 >> y0;
    goalPosition.push_back(w0);
    goalPosition.push_back(x0);
    goalPosition.push_back(y0);

    for (int win = 0; win < numWindows; win++) {
        ifile >> x >> y >> w >> h;
        int** arr = new int*[h/32];
        for (int i = 0; i < h/32; i++) {
            arr[i] = new int[w/32];
            for (int j = 0; j < w/32; j++) {
                ifile >> arr[i][j];
            }
        }
        if (!newWindow(filename, x, y, w, h)) return false;
        TextureMap* newMap = new TextureMap(windows[win], arr);
        maps.push_back(newMap);
        ifile >> numColliders;
        for (int c = 0; c < numColliders; c++) {
            SDL_Rect temp;
            ifile >> temp.x >> temp.y >> temp.w >> temp.h;
            windows[win]->absoluteColliders.push_back(temp);
            windows[win]->relativeColliders.push_back(temp);
        }
        ifile >> numHazards;
        for (int h = 0; h < numHazards; h++) {
            ifile >> x >> y;
            Hazard* newHazard = new Hazard(windows[win], x, y);
            hazards.push_back(newHazard);
        }
        if (win == goalPosition[0]) {
            goal = new Goal(windows[win], goalPosition[1], goalPosition[2]);
        }
        windows[win]->index = win;
    }
    setActiveWindow(startingPosition[0]);
    updateRects();
    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        controller->addEvent(event);
        if (controller->isPressed("ESCAPE")) {
            isRunning = false;
        }
        else {
            if (controller->isPressed("A") && !controller->isPressed("D")) {
                player->setxState(MOVING_LEFT);
            }
            else if (!controller->isPressed("A") && controller->isPressed("D")) {
                player->setxState(MOVING_RIGHT);
            }
            else {
                player->setxState(IDLE);
            }
            if (controller->isPressed("SPACE")) {
                player->jump();
            }
        }
    }
}

void Game::updateRects() {
    int tempx, tempy;
    // Get active window position
    activeWindow->rect.x = 0;
    activeWindow->rect.y = 0;
    SDL_GetWindowPosition(activeWindow->window, &tempx, &tempy);

    // Update all window relative collider, relative edge and corner guard positions
    for (auto& w: windows) {
        SDL_GetWindowPosition(w->window, &w->rect.x, &w->rect.y);
        w->rect.x -= tempx;
        w->rect.y -= tempy;
        for (unsigned int c = 0; c < w->relativeColliders.size(); c++) {
            w->relativeColliders[c].x = w->absoluteColliders[c].x + w->rect.x;
            w->relativeColliders[c].y = w->absoluteColliders[c].y + w->rect.y;
        }
        if (w != activeWindow && Collision::AABB(activeWindow->rect, w->rect)) {
            w->valid = false;
        }
        else w->valid = true;
    }
}

void Game::update() {
    int tempx, tempy;
    // Get active window position
    activeWindow->rect.x = 0;
    activeWindow->rect.y = 0;
    SDL_GetWindowPosition(activeWindow->window, &tempx, &tempy);

    // Update all window relative collider, relative edge and corner guard positions
    for (auto& w: windows) {
        SDL_GetWindowPosition(w->window, &w->rect.x, &w->rect.y);
        w->rect.x -= tempx;
        w->rect.y -= tempy;
        for (unsigned int c = 0; c < w->relativeColliders.size(); c++) {
            w->relativeColliders[c].x = w->absoluteColliders[c].x + w->rect.x;
            w->relativeColliders[c].y = w->absoluteColliders[c].y + w->rect.y;
        }
        if (w != activeWindow && Collision::AABB(activeWindow->rect, w->rect)) {
            w->valid = false;
        }
        else w->valid = true;
    }

    if (player != nullptr) {
        player->update();
    }

    for (auto& h: hazards) {
        h->update(player);
    }

    if (goal != nullptr) {
        goal->update(player);
    }

    if (!player->isAlive()) {
        delete player;
        setActiveWindow(startingPosition[0]);
        player = new Player(startingPosition[1], startingPosition[2]);
    }
}

void Game::render() {
    for (std::vector<Window*>::iterator it = Game::windows.begin(); it != Game::windows.end(); ++it) {
        SDL_RenderClear((*it)->renderer);
    }
    for (std::vector<TextureMap*>::iterator it = maps.begin(); it != maps.end(); ++it) {
        (*it)->render();
    }
    for (auto& h: hazards) {
        h->render();
    }
    goal->render();
    player->render();
    for (std::vector<Window*>::iterator it = Game::windows.begin(); it != Game::windows.end(); ++it) {
        SDL_RenderPresent((*it)->renderer);
    }
}

void Game::closeAll() {
    // Fixed memory error: Deallocate maps before the windows they are attached to
    while (!maps.empty()) {
        delete maps[maps.size()-1];
        maps.pop_back();
    }
    for (std::vector<Window*>::iterator it = windows.begin(); it < windows.end(); ++it) {
        if ((*it)->window != nullptr) {
            SDL_DestroyWindow((*it)->window);
            (*it)->window = nullptr;
            std::cout << "Window destroyed" << std::endl;
        }
        if ((*it)->renderer != nullptr) {
            SDL_DestroyRenderer((*it)->renderer);
            (*it)->renderer = nullptr;
            std::cout << "Renderer destroyed" << std::endl;
        }
        delete (*it);
    }
    windows.clear();
}

void Game::cleanup() {
    // Deallocate items in the reverse order they were allocated
    delete controller;
    delete player;
    while(!hazards.empty()) {
        delete hazards[hazards.size() - 1];
        hazards.pop_back();
    }
    delete goal;
    closeAll();
    SDL_Quit();
    std::cout << "SDL quit" << std::endl;
}

bool Game::running() {
    return isRunning;
}

void Game::setActiveWindow(int x) {
    activeWindow = windows[x];
    activeWindowIndex = x;
}