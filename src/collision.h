#ifndef COLLISION_H
#define COLLISION_H
#include "game.h"

class Collision {
    public:
        static bool AABB(const SDL_Rect& A, const SDL_Rect& B);
        static bool AABBinc(const SDL_Rect& A, const SDL_Rect& B);
        static bool bounded(const SDL_Rect& A, const SDL_Rect& B); // A is bounded by B
};

#endif