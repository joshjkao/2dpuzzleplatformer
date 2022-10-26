#include "collision.h"

bool Collision::AABB(const SDL_Rect& A, const SDL_Rect& B) {
    if (A.x + A.w > B.x &&
        B.x + B.w > A.x &&
        A.y + A.h > B.y &&
        B.y + B.h > A.y) {
            return true;
        }
    return false;
}

bool Collision::AABBinc(const SDL_Rect& A, const SDL_Rect& B) {
    if (A.x + A.w >= B.x &&
        B.x + B.w >= A.x &&
        A.y + A.h >= B.y &&
        B.y + B.h >= A.y) {
            return true;
        }
    return false;
}

bool Collision::bounded(const SDL_Rect& A, const SDL_Rect& B) {
    if ( A.x >= B.x &&
        (A.x + A.w) <= (B.x + B.w) &&
         A.y >= B.y &&
        (A.y + A.h) <= (B.y + B.h)) {
            return true;
        }
    return false;
}