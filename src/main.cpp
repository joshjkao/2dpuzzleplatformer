#include "game.h"
#include "leveleditor.h"

Game* game = nullptr;
LevelEditor* editor = nullptr;

int main(int argc, char** argv) {
    const int fps = 60;
    const int frameDelay = 1000/fps;
    Uint32 frameStart;
    int frameTime;
    if (argc > 1 && (std::strcmp(argv[1], "LEVELEDITOR") == 0)) {
        std::cout << "Started in editing mode" << std::endl;
        editor = new LevelEditor();
        if (!editor->init()) return -1;

        while (editor->running()) {
            frameStart = SDL_GetTicks();

            // Check for inputs
            editor->handleEvents();

            // Update object states
            editor->update();

            // Draw new frame
            editor->render();

            // Frame rate limiter
            frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay-frameTime);
            }
        }

        editor->cleanup();
        delete editor;
        return 0;
    }
    else {
        game = new Game();
        if (!game->init("SDL Window", 100, 100, 480, 320)) return -1;

        // Main loop
        while (game->running()) {
            frameStart = SDL_GetTicks();

            // Check for inputs
            game->handleEvents();

            // Update object states
            game->update();

            // Draw new frame
            game->render();

            // Frame rate limiter
            frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay-frameTime);
            }
        }

        game->cleanup();
        delete game;
        return 0;
    } 
}