#ifndef TEXTUREMANGER_H
#define TEXTUREMANGER_H
#include "game.h"

class TextureManager {
    public:
        static std::vector<SDL_Texture*> loadTexture(const char* filename);
        static void render(Window* window, SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip = SDL_FLIP_NONE);
        static SDL_Texture* loadUniqueTexture(Window* window, const char* filename);
};

#endif