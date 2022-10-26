#include "keyboardcontroller.h"
#include <sstream>

KeyboardController::KeyboardController() {
    std::cout << "created a keyboard controller" << std::endl;
}

KeyboardController::~KeyboardController() {

}

void KeyboardController::addEvent(SDL_Event event) {
    switch (event.type) {
        case SDL_KEYDOWN: {
            if (event.key.keysym.scancode == SDL_SCANCODE_A) keysDown.emplace("A");
            if (event.key.keysym.scancode == SDL_SCANCODE_D) keysDown.emplace("D");
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) keysDown.emplace("SPACE");
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) keysDown.emplace("ESCAPE");
            break;
        }
        case SDL_KEYUP: {
            if (event.key.keysym.scancode == SDL_SCANCODE_A) keysDown.erase("A");
            if (event.key.keysym.scancode == SDL_SCANCODE_D) keysDown.erase("D");
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) keysDown.erase("SPACE");
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) keysDown.erase("ESCAPE");
            break;
        }
        case SDL_WINDOWEVENT: {
            // Moving a window causes the player to stop moving
            keysDown.clear();
            break;
        }
        default:
            break;
    }
}

bool KeyboardController::isPressed(const char* key) {
    if (keysDown.find(key) != keysDown.end()) {
        return true;
    }
    else return false;
}

bool KeyboardController::arePressed(const char* key) {
    std::stringstream ss(key);
    std::string s;
    ss >> s;
    while (!ss.fail()) {
        if (s[0] == '!') {
            s.erase(0,1);
            if (isPressed(s.c_str())) {
                std::cout << s << " should not be pressed" << std::endl;
                return false;
            }
            else std::cout << '!' << s << std::endl;
        }
        else {
            if (!isPressed(s.c_str())) {
                std::cout << s << " should be pressed" << std::endl;
                return false;
            }
            else std::cout << s << std::endl;
        }
        ss >> s;
    }
    return true;
}