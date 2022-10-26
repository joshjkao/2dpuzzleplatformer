#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Window {
    Window();
    Window(const Window&);
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect rect;
    bool valid = true;
    std::vector<SDL_Rect> relativeColliders;
    std::vector<SDL_Rect> absoluteColliders;
    int index;
};

class Game {
    public:
        // Allocation and deletion
        Game();
        ~Game();

        // Window and game setup
        bool init(const char* title, int xpos, int ypos, int width, int height);
        bool newWindow(const char* title, int xpos, int ypos, int width, int height);
        bool loadLevel(const char* filename);

        // Main game loop
        void handleEvents();
        void updateRects();
        void update();
        void render();
        bool running();

        // Window and game cleanup
        void closeAll();
        void cleanup();
        
        // Public data members: Window, Renderer, dimensions for each window
        static std::vector<Window*> windows;
        static int activeWindowIndex;
        static Window* activeWindow;

        // Active window depends on which window the player is in, gets updated by player
        static void setActiveWindow(int x);
    private:
        bool SDL_init();
        bool isRunning;
        std::vector<int> startingPosition;
};

#endif

/* TO DO
- Move fstream level parser to a class, use game.cpp to call a static member function
- Remove global variables
- Add functionality for closing all windows, starting a new level (probably part of Game class)
- Add game hazards, win condition
- Find an m1 compatible memory checker
- Start working on animations, art for backgrounds
- Different biomes/themes for different windows? Different hazards depending on theme?
- Or make everything look like signs?
- Generalize Player class: change Entity to GameObject class, create new Entity class inheriting from GameObject
- create Entity Component System
- Sound design?
- Menu and other UI


*/
