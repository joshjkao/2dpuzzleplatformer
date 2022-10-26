#ifndef PLAYER_H
#define PLAYER_H
#include "game.h"
#include "entity.h"
#include "vector2d.h"

enum PlayerState {IDLE, MOVING_LEFT, MOVING_RIGHT, AIRBORNE, GROUNDED};

class Player: public Entity {
    public:
        // Creation and deletion
        Player(int x = 0, int y = 0);
        ~Player();

        // Update positions, draw
        void update();
        void render();
        
        // Perform actions
        void kill();
        void setxState(PlayerState state);
        void jump();

        // Access private attributes
        bool isAlive();
        SDL_Rect getDest_Rect(int index);
        
    private:
        PlayerState xState = IDLE;
        PlayerState yState = GROUNDED;
        double xvel = 0;
        double yvel = 0;
        double vmax = 4;
        double a = 0.35;
        
        bool alive = true;
        int killZone = INT_MAX;

        bool checkValidPosition(SDL_Rect* next, SDL_Rect* &result);

        void play(const char* animName);
};

#endif