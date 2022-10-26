#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H
#include "game.h"
#include <set>

// Keeps a set of keys that have been pressed
class KeyboardController {
    public:
        KeyboardController();
        ~KeyboardController();

        bool isPressed(const char*);
        bool arePressed(const char*);
        void addEvent(SDL_Event);

    private:
        std::set<const char*> keysDown;
};

#endif