#include "texturemanager.h"
#include <iostream>

std::vector<SDL_Texture*> TextureManager::loadTexture(const char* filename) {
    SDL_Surface* temp = IMG_Load(filename);
    std::cout << "loaded image" << std::endl;
    std::vector<SDL_Texture*> output;
    std::cout << "created texture" << std::endl;
    for (int i = 0; i < Game::windows.size(); i++) {
        output.push_back(SDL_CreateTextureFromSurface(Game::windows[i]->renderer, temp));
        std::cout << "created texture" << std::endl;
    }
    SDL_FreeSurface(temp);

    return output;
}

void TextureManager::render(Window* window, SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(window->renderer, texture, &src, &dest, 0, NULL, flip);
}  

SDL_Texture* TextureManager::loadUniqueTexture(Window* window, const char* filename) {
    SDL_Surface* temp = IMG_Load(filename);
    std::cout << "loaded image" << std::endl;
    SDL_Texture* output = SDL_CreateTextureFromSurface(window->renderer, temp);
    std::cout << "created unique texture" << std::endl;
    SDL_FreeSurface(temp);
    return output;
}