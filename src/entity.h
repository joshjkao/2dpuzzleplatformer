#ifndef ENTITY_H
#define ENTITY_H
#include "game.h"
#include "texturemanager.h"
#include "collision.h"
#include "animation.h"
#include <map>

// Entity is an abstract class inherited by Player, Goal, and Hazard
class Entity {
    public: 
        Entity(const char* texturefile, int x, int y, int w = 32, int h = 32);
        virtual ~Entity();

        virtual void update() = 0;
        virtual void render() = 0;

        void updateRects();

    protected:
        int xpos;
        int ypos;
        bool animated = false;

        SDL_RendererFlip flip = SDL_FLIP_NONE;
        int animIndex = 0;
        int frames = 1;
        int speed= 1;

        std::vector<SDL_Texture*> textures;
        SDL_Rect src_rect;
        std::vector<SDL_Rect> dest_rects;
        std::map<const char*, Animation> animations;

};

#endif