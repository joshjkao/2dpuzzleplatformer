#include "entity.h"

Entity::Entity(const char* texturefile, int x, int y, int w, int h):
xpos(x), ypos(y), textures(TextureManager::loadTexture(texturefile))
{
    SDL_Rect src;
    src.x = src.y = 0;
    src.w = w;
    src.h = h;
    src_rect = src;
    // SDL_Rect rect1;
    // rect1.x = x;
    // rect1.y = y;
    // rect1.w = w;
    // rect1.h = h;
    // dest_rects.push_back(rect1);
    for (int i = 0; i < Game::windows.size(); i++) {
        SDL_Rect rect;
        rect.x = -64;
        rect.y = -64;
        rect.w = w;
        rect.h = h;
        dest_rects.push_back(rect);
    }
    std::cout << "created entity" << std::endl;
}

Entity::~Entity() {
    std::cout << "entity destroyed" << std::endl;
}

void Entity::updateRects() {
    for (unsigned int i = 0; i < Game::windows.size(); i++) {
        if (i == Game::activeWindowIndex || !Game::windows[i]->valid) continue;
        dest_rects[i].x = xpos - Game::windows[i]->rect.x;
        dest_rects[i].y = ypos - Game::windows[i]->rect.y;
    }
}