#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H
#include "game.h"

class TextureMap {
    public:
        TextureMap(Window*, int**);
        TextureMap(const TextureMap&);
        ~TextureMap();

        void loadMap(int**);
        void render();

    private:
        SDL_Rect src;
        SDL_Rect dest;
        std::vector <SDL_Texture*> textures;
        int** array;
        // std::vector<std::vector<int> > vect;
        Window* window;
};

#endif
