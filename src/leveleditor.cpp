#include "leveleditor.h"
#include "keyboardcontroller.h"
#include <iostream>
#include <fstream>

SDL_Event LevelEditor::event;

LevelEditor::LevelEditor() {

}

LevelEditor::~LevelEditor() {
    
}

bool LevelEditor::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        std::cout << "SDL_Init failed" << std::endl;
        isRunning = false;
        return false;
    }
    std::cout << "SDL initialized" << std::endl;
    mainWindow.window = SDL_CreateWindow("Level Editor", 0, 0, 1000, 700, SDL_WINDOW_OPENGL);
    if (mainWindow.window == nullptr) {
        SDL_Quit();
        isRunning = false;
        return false;
    }
    mainWindow.renderer = SDL_CreateRenderer(mainWindow.window, -1, SDL_RENDERER_PRESENTVSYNC);
    isRunning = true;
    return true;
}

void LevelEditor::cleanup() {
    if (mainWindow.renderer != nullptr) {
        SDL_DestroyRenderer(mainWindow.renderer);
        mainWindow.renderer = nullptr;
    }
    if (mainWindow.window != nullptr) {
        SDL_DestroyWindow(mainWindow.window);
        mainWindow.window = nullptr;
    }
    for (auto& w: editorWindows) {
        for (auto& i: w->items) {
            delete i;
        }
        delete w;
    }
    SDL_Quit();
}

void LevelEditor::handleEvents() {
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN: {
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W: {
                        addWindow();
                        break;
                    }
                    case SDL_SCANCODE_C: {
                        addCollider();
                        break;
                    }
                    case SDL_SCANCODE_H: {
                        addHazard();
                        break;
                    }
                    case SDL_SCANCODE_P: {
                        addPlayer();
                        break;
                    }
                    case SDL_SCANCODE_G: {
                        addGoal();
                        break;
                    }
                    case SDL_SCANCODE_RIGHT: {
                        selectWindow(1);
                        std::cout << currentWindow << std::endl;
                        break;
                    }
                    case SDL_SCANCODE_LEFT: {
                        selectWindow(-1);
                        std::cout << currentWindow << std::endl;
                        break;
                    }
                    case SDL_SCANCODE_S: {
                        toggleGridSnap();
                        break;
                    }
                    case SDL_SCANCODE_ESCAPE: {
                        saveMap();
                        isRunning = false;
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case SDL_QUIT: {
                isRunning = false;
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                if (!waitingInput) {
                    SDL_GetMouseState(&mousePos1[0], &mousePos1[1]);
                    if (snap) getGridCoords(mousePos1[0], mousePos1[1]);
                    waitingInput = true;
                    std::cout << "placed marker 1 at " << mousePos1[0] << ", " << mousePos1[1] << std::endl;
                }
                else {
                    SDL_GetMouseState(&mousePos2[0], &mousePos2[1]);
                    if (snap) getGridCoords(mousePos2[0], mousePos2[1]);
                    waitingInput = false;
                    std::cout << "placed marker 2 at " << mousePos2[0] << ", " << mousePos2[1] << std::endl;
                }
                break;
            }
            default: {
                break;
            }
        }
    }
}

void LevelEditor::update() {
    // for (auto& w: editorWindows) {
    //     for (auto& c: w->colliders) {
            
    //     }
    //     for (auto& h: w->colliders) {

    //     }
    // }
}

void LevelEditor::render() {
    SDL_SetRenderDrawColor(mainWindow.renderer, 0, 0, 0, 255);
    SDL_RenderClear(mainWindow.renderer);
    int ind = 0;
    for (auto& w: editorWindows) {
        if (ind == currentWindow) {
            SDL_Rect highlight(w->rect);
            highlight.x -= 1;
            highlight.y -= 1;
            highlight.w += 2;
            highlight.h += 2;
            SDL_SetRenderDrawColor(mainWindow.renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(mainWindow.renderer, &highlight);
        }
        SDL_SetRenderDrawColor(mainWindow.renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(mainWindow.renderer, &w->rect);
        for (auto& c: w->colliders) {
            SDL_SetRenderDrawColor(mainWindow.renderer, 0, 0, 200, 255);
            SDL_RenderFillRect(mainWindow.renderer, &c->rect);
        }
        for (auto& h: w->hazards) {
            SDL_SetRenderDrawColor(mainWindow.renderer, 150, 0, 0, 255);
            SDL_RenderFillRect(mainWindow.renderer, &h->rect);
        }
        ind++;
    }
    if (player != nullptr) {
        SDL_SetRenderDrawColor(mainWindow.renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(mainWindow.renderer, &player->rect);
    }
    if (goal != nullptr) {
        SDL_SetRenderDrawColor(mainWindow.renderer, 50, 100, 50, 255);
        SDL_RenderFillRect(mainWindow.renderer, &goal->rect);
    }
    SDL_RenderPresent(mainWindow.renderer);
}

bool LevelEditor::running() {
    return isRunning;
}

void LevelEditor::addWindow() {
    if ((mousePos1[0] - mousePos2[0]) * (mousePos1[1] - mousePos2[1]) == 0) {
        std::cout << "cannot create a rectangle of area 0" << std::endl;
        return;
    }
    EditorWindow* newWindow = new EditorWindow();
    newWindow->rect.x = mousePos1[0];
    newWindow->rect.y = mousePos1[1];
    newWindow->rect.w = mousePos2[0] - mousePos1[0];
    newWindow->rect.h = mousePos2[1] - mousePos1[1];
    editorWindows.push_back(newWindow);
    currentWindow = editorWindows.size() - 1;
    return;
}

void LevelEditor::addCollider() {
    if (editorWindows.size() == 0) {
        std::cout << "no window selected" << std::endl;
        return;
    }
    if ((mousePos1[0] - mousePos2[0]) * (mousePos1[1] - mousePos2[1]) == 0) {
        std::cout << "cannot create a rectangle of area 0" << std::endl;
        return;
    }
    EditorCollider* newCollider = new EditorCollider(mousePos1[0], mousePos1[1]);
    newCollider->rect.w = mousePos2[0] - mousePos1[0];
    newCollider->rect.h = mousePos2[1] - mousePos1[1];
    editorWindows[currentWindow]->colliders.push_back(newCollider);
    editorWindows[currentWindow]->items.push_back(newCollider);
    return;
}

void LevelEditor::addHazard() {
    if (editorWindows.size() == 0) {
        std::cout << "no window selected" << std::endl;
        return;
    }
    EditorHazard* newHazard = new EditorHazard(mousePos1[0], mousePos1[1]);
    editorWindows[currentWindow]->hazards.push_back(newHazard);
    editorWindows[currentWindow]->items.push_back(newHazard);
    return;
}

void LevelEditor::addPlayer() {
    if (editorWindows.size() == 0) {
        std::cout << "cannot create a player without a window" << std::endl;
    }
    if (player != nullptr) {
        std::cout << "player already exists" << std::endl;
        return;
    }
    player = new EditorPlayer(mousePos1[0], mousePos1[1]);
    player->startingWindow = currentWindow;
    return;
}

void LevelEditor::addGoal() {
    if (editorWindows.size() == 0) {
        std::cout << "cannot create a goal without a window" << std::endl;
    }
    if (goal != nullptr) {
        delete goal;
        goal = nullptr;
        return;
    }
    goal = new EditorGoal(mousePos1[0], mousePos1[1]);
    goal->startingWindow = currentWindow;
    return;
}

void LevelEditor::saveMap() {
    std::ofstream ofile("levels/newlevel.txt");
    ofile << editorWindows.size() << std::endl;

    if (player == nullptr) {
        std::cout << "map missing player" << std::endl;
        return;
    }
    ofile << player->startingWindow << " " << player->rect.x - editorWindows[player->startingWindow]->rect.x;
    ofile << " " << player->rect.y - editorWindows[player->startingWindow]->rect.y << std::endl;
    
    if (goal == nullptr) {
        std::cout << "map missing goal" << std::endl;
        return;
    }
    ofile << goal->startingWindow << " " << goal->rect.x - editorWindows[goal->startingWindow]->rect.x;
    ofile << " " << goal->rect.y - editorWindows[goal->startingWindow]->rect.y << std::endl;
    
    for (auto& w: editorWindows) {
        ofile << w->rect.x << " " << w->rect.y << " "  << w->rect.w << " "  << w->rect.h << std::endl;
        for (int i = 0; i < w->rect.h/32; i++) {
            for (int j = 0; j < w->rect.w/32; j++) {
                ofile << "8 ";
            }
            ofile << std::endl;
        }
        ofile << w->colliders.size() << std::endl;
        for (auto& c: w->colliders) {
            ofile << c->rect.x - w->rect.x << " " << c->rect.y - w->rect.y << " " << c->rect.w << " " << c->rect.h << std::endl;
        }
        ofile << w->hazards.size() << std::endl;
        for (auto& h: w->hazards) {
            ofile << h->rect.x - w->rect.x << " " << h->rect.y - w->rect.y << std::endl;
        }
        ofile << std::endl << std::endl;
    }
    
    return;
}

void LevelEditor::selectWindow(int i) {
    if (editorWindows.size() == 0) return;
    currentWindow += i;
    if (currentWindow == editorWindows.size()) {
        currentWindow = 0;
    }
    else if (currentWindow < 0) {
        currentWindow = editorWindows.size() -1;
    }
    currentItem = 0;
    return;
}

void LevelEditor::selectItem(int i) {
    if (editorWindows[currentWindow]->items.size() == 0) return;
    currentItem += i;
    if (currentItem == editorWindows[currentWindow]->items.size()) {
        currentWindow = 0;
    }
    else if (currentWindow < 0) {
        currentWindow = editorWindows[currentWindow]->items.size() - 1;
    }
    return;
}

void LevelEditor::toggleGridSnap() {
    snap = !snap;
    std::cout << "snap is " << snap << std::endl;
    return;
}

void LevelEditor::getGridCoords(int& x, int& y) {
    x = x/32;
    y = y/32;
    x *= 32;
    y *= 32;
    return;
}