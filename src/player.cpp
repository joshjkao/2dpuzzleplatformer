#include "player.h"
#include "texturemanager.h"
#include <cmath>

Player::Player(int x, int y):
Entity("assets/playertest1.png",x,y,32,64), a(0.35)
{
    src_rect.w = 64;
    Animation idle(0, 1, 100);
    Animation walk(1, 8, 45);
    animations.emplace("Idle", idle);
    animations.emplace("Walk", walk);

    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        std::cout << "failed to get display mode" << std::endl;
    }
    killZone = dm.h + 32;
    std::cout << "created player" << std::endl;
}

Player::~Player() {
    std::cout << "player destroyed" << std::endl;
}

void Player::update() {
    if (ypos >= killZone) {
        std::cout << "Player fell out of the world" << std::endl;
        kill();
        return;
    }
    

    // Update x velocity (according to keyboard events)
    switch (xState) {
        case MOVING_LEFT: {
            if (xvel-a <= -vmax) xvel = -vmax;
            else xvel -= a;
            break;
        }
        case MOVING_RIGHT: {
            if (xvel+a >= vmax) xvel = vmax;
            else xvel += a;
            break;
        }
        case IDLE: {
            if (xvel == 0) break;
            else if (xvel > 0) {
                if (xvel - a < 0) xvel = 0;
                else xvel -= a;
            }
            else {
                if (xvel + a > 0) xvel = 0;
                else xvel += a;
            }
        }
        default: 
            break;
    }
    // Update y velocity (always the same)
    yvel += a;
    
    // Test new x position for collisions (testing x first is favorable)
    SDL_Rect xnext(dest_rects[Game::activeWindowIndex]);
    xnext.x += xvel;
    SDL_Rect* result = nullptr;
    if (!checkValidPosition(&xnext, result)) {
        if (xvel > 0) { // Collision in x direction on the right side
            xvel = result->x - dest_rects[Game::activeWindowIndex].x - dest_rects[0].w;
        }
        else { // Collision in x direction on the left side
            xvel = result->x + result->w - xpos;
        }
    }

    // Test new y position AFTER ADDING NEW X COMPONENT 
    SDL_Rect ynext(dest_rects[Game::activeWindowIndex]);
    ynext.x += xvel;
    ynext.y += yvel;
    result = nullptr;
    if (!checkValidPosition(&ynext, result)) {
        if (yvel > 0) { // Collision on bottom
            yvel = result->y - dest_rects[Game::activeWindowIndex].y - dest_rects[0].h;
            yState = GROUNDED; // Make the player able to jump again
        }
        else { // Collision on top
            yvel = result->y + result->h - ypos;
        }
    }
    else if (abs(yvel) > 2) {
        yState = AIRBORNE;
    }

    // Update position data members and active window rectangle
    ypos += yvel;
    dest_rects[Game::activeWindowIndex].y = ypos;
    xpos += xvel;
    dest_rects[Game::activeWindowIndex].x = xpos;

    // Update secondary window rects and change active windows if new location is fully bounded by a new window
    bool changeActiveWindow = false;
    int newActiveWindow = Game::activeWindowIndex;
    for (unsigned int i = 0; i < Game::windows.size(); i++) {
        if (i == Game::activeWindowIndex || !Game::windows[i]->valid) continue;
        dest_rects[i].x = xpos - Game::windows[i]->rect.x;
        dest_rects[i].y = ypos - Game::windows[i]->rect.y;
        if (Collision::bounded(dest_rects[Game::activeWindowIndex], Game::windows[i]->rect)) {
            changeActiveWindow = true;
            newActiveWindow = i;
        }
    }
    if (changeActiveWindow) {
        Game::setActiveWindow(newActiveWindow);
        xpos = dest_rects[newActiveWindow].x;
        ypos = dest_rects[newActiveWindow].y;
    }

    // Preparing for rendering
    if (xvel > 0) {
        flip = SDL_FLIP_NONE;
    }
    else if (xvel < 0) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    if ( yState == AIRBORNE) {
        play("Idle");
    }
    else if (abs(xvel) > 2) {
        play("Walk");
    }
    else {
        play("Idle");
    }
}

void Player::render() {
    src_rect.y = animIndex * src_rect.h;
    src_rect.x = src_rect.w * static_cast<int>((SDL_GetTicks()/speed) % frames);
    for (unsigned int i = 0; i < Game::windows.size(); i++) {
        if (!Game::windows[i]->valid) continue;
        SDL_Rect temp = dest_rects[i];
        temp.x -= 16;
        temp.w += 32;
        TextureManager::render(Game::windows[i], textures[i], src_rect, temp, flip);
    } 
}

void Player::setxState(PlayerState state) {
    if (state == MOVING_LEFT) xState = state;
    else if (state == MOVING_RIGHT) xState = state;
    else if (state == IDLE) xState = state;
    else return;
}

void Player::jump() {
    if (yState == GROUNDED) {
        yvel = -7;
        yState = AIRBORNE;
    }
}

bool Player::checkValidPosition(SDL_Rect* next, SDL_Rect* &result) {
    bool collision = false;
    result = nullptr;
    for (auto& w: Game::windows) {
        if (!w->valid) continue;
        for (auto& c: w->relativeColliders) {
            if (Collision::AABB(*next, c)) {
                collision = true;
                result = &c;
                break;
            }
        }
        if (collision) break;
    }
    return !collision;
}

void Player::play(const char* animName) {
    if (animations.find(animName) == animations.end()) {
        std::cout << "animation not found" << std::endl;
    }
    else {
        frames = animations[animName].a_frames;
        speed = animations[animName].a_speed;
        animIndex = animations[animName].a_index;
    }
}

bool Player::isAlive() {
    return alive;
}

void Player::kill() {
    alive = false;
}

SDL_Rect Player::getDest_Rect(int index) {
    return dest_rects[index];
}