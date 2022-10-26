#include "interactables.h"
#include "collision.h"

Hazard::Hazard(Window* window_, int x_, int y_):
Entity("assets/pizza.png",x_,y_), window(window_)
{
    dest_rects[0].x = x_;
    dest_rects[0].y = y_;
    std::cout << "created hazard" << std::endl;
}

Hazard::~Hazard() {

}

void Hazard::update() {

}

void Hazard::update(Player* player) {
    if (!window->valid) return;
    else if (Collision::AABB(dest_rects[0], player->getDest_Rect(window->index))) {
        player->kill();
    }
}

void Hazard::render() {
    SDL_RenderCopy(window->renderer, textures[window->index], NULL, &dest_rects[0]);
    // TextureManager::render(window, textures[window->index], src_rect, dest_rects[window->index]);
}



Goal::Goal(Window* window_, int x_, int y_):
Entity("assets/pizza.png",x_,y_,32,64), window(window_)
{
    dest_rects[0].x = x_;
    dest_rects[0].y = y_;
    std::cout << "created goal" << std::endl;
}

Goal::~Goal() {

}

void Goal::update() {

}

void Goal::update(Player* player) {
    if (!window->valid) return;
    if (Collision::AABB(dest_rects[0], player->getDest_Rect(window->index))) {
        std::cout << "goal hit" << std::endl;
    }
}

void Goal::render() {
    SDL_RenderCopy(window->renderer, textures[window->index], NULL, &dest_rects[0]);
    // TextureManager::render(window, textures[window->index], src_rect, dest_rects[window->index]);
}