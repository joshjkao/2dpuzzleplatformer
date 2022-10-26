#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include <vector>
#include <SDL2/SDL.h>
#include "game.h"

enum Direction {LEFT, RIGHT, UP, DOWN};

class EditorItem {
    public:
        EditorItem(int x, int y) {
            rect.x = x;
            rect.y = y;
        }
        virtual ~EditorItem() {

        }
        SDL_Rect rect;
        void nudge(Direction dir) {
            switch (dir) {
                case LEFT: {
                    rect.x -= 16;
                    break;
                }
                case RIGHT: {
                    rect.x += 16;
                    break;
                }
                case UP: {
                    rect.y -= 16;
                    break;
                }
                case DOWN: {
                    rect.y += 16;
                    break;
                }
            }
        }
};

class EditorPlayer: public EditorItem {
    public:
        EditorPlayer(int x, int y): EditorItem(x, y) {
            rect.h = 64;
            rect.w = 32;
        }
        int startingWindow = 0;
};

class EditorGoal: public EditorItem {
    public:
        EditorGoal(int x, int y): EditorItem(x, y) {
            rect.h = 64;
            rect.w = 32;
        }
        int startingWindow = 0;
};

class EditorCollider: public EditorItem {
    public:
        EditorCollider(int x, int y): EditorItem(x, y) {

        }
};

class EditorHazard: public EditorItem {
    public:
        EditorHazard(int x, int y): EditorItem(x, y) {
            rect.h = 32;
            rect.w = 32;
        }
};

struct EditorWindow {
    SDL_Rect rect;
    std::vector<EditorItem*> items;
    std::vector<EditorCollider*> colliders;
    std::vector<EditorHazard*> hazards;
};

class LevelEditor {
    public:
        // Allocation/Deletion
        LevelEditor();
        ~LevelEditor();
        bool init();
        void cleanup();

        // Main Game Loop
        void handleEvents();
        void update();
        void render();
        bool running();

    private:
        // Editing Functions
        void addPlayer();
        void addGoal();
        void addCollider();
        void addHazard();
        void addWindow();
        void saveMap();
        void selectWindow(int);
        void selectItem(int);
        void toggleGridSnap();

        EditorPlayer* player = nullptr;
        EditorGoal* goal = nullptr;
        std::vector<EditorWindow*> editorWindows;

        int currentWindow = -1;
        int currentItem = -1;

        static SDL_Event event;

        std::vector<int> mousePos1 = {0,0};
        std::vector<int> mousePos2 = {0,0};

        Window mainWindow;
        bool isRunning;

        bool waitingInput = false;

        void getGridCoords(int& x, int& y);

        bool snap = true;
};

#endif