#ifndef INTERACTABLES_H
#define INTERACTABLES_H
#include "game.h"
#include "entity.h"
#include "player.h"

class Hazard: public Entity {
    public:
        Hazard(Window*, int x = 0, int y = 0);
        ~Hazard();
        void update();
        void update(Player* player);
        void render();
    private:
        Window* window;
};

class Goal: public Entity {
    public:
        Goal(Window*, int x = 0, int y = 0);
        ~Goal();
        void update();
        void update(Player* player);
        void render();
    private:
        Window* window;
};

#endif