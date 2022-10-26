#include "texturemap.h"
#include "texturemanager.h"

TextureMap::TextureMap(Window* win_, int** arr):
array(arr), window(win_)
{
    textures.push_back(TextureManager::loadUniqueTexture(win_, "assets/0.png"));
    textures.push_back(TextureManager::loadUniqueTexture(win_, "assets/1.png"));
    textures.push_back(TextureManager::loadUniqueTexture(win_, "assets/2.png"));
    textures.push_back(TextureManager::loadUniqueTexture(win_, "assets/3.png"));
    textures.push_back(TextureManager::loadUniqueTexture(win_, "assets/4.png"));
    textures.push_back(TextureManager::loadUniqueTexture(win_, "assets/5.png"));
    textures.push_back(TextureManager::loadUniqueTexture(win_, "assets/6.png"));
    textures.push_back(TextureManager::loadUniqueTexture(win_, "assets/7.png"));

    src.x = src.y = 0;
    src.h = src.w = 32;
    dest.x = dest.y = 0;
    dest.h = dest.w = 32;

    std::cout << "created texture map of size " << win_->rect.h << "x" << win_->rect.w << std::endl;
}

TextureMap::TextureMap(const TextureMap&) {
    std::cout << "copy called" << std::endl;
}

TextureMap::~TextureMap() {
    for (int i = 0; i < window->rect.h/32; i++) {
        delete [] array[i];
    }
    delete [] array;
    std::cout << "texturemap destroyed" << std::endl;
}

void TextureMap::loadMap(int** arr) {
    for (int i = 0; i < window->rect.h/32; i++) {
        for (int j = 0; j < window->rect.w/32; j++) {
            array[i][j] = arr[i][j];
        }
    }
}

void TextureMap::render() {
    int id = 0;
    for (int i = 0; i < window->rect.h/32; i++) {
        for (int j = 0; j < window->rect.w/32; j++) {
            id = array[i][j];
            dest.x = j * 32;
            dest.y = i * 32;
            switch (id)
            {
            case 0: {
                TextureManager::render(window, textures[0], src, dest);
                break;
            }
            case 1: {
                TextureManager::render(window, textures[1], src, dest);
                break;
            }
            case 2: {
                TextureManager::render(window, textures[2], src, dest);
                break;
            }
            case 3: {
                TextureManager::render(window, textures[3], src, dest);
                break;
            }
            case 4: {
                TextureManager::render(window, textures[4], src, dest);
                break;
            }
            case 5: {
                TextureManager::render(window, textures[5], src, dest);
                break;
            }
            case 6: {
                TextureManager::render(window, textures[6], src, dest);
                break;
            }
            case 7: {
                TextureManager::render(window, textures[7], src, dest);
                break;
            }
            default: {
                break;
            }
            }
        }
    }
}