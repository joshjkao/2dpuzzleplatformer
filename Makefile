SRC_DIR = src
BUILD_DIR = build/debug
CC=g++
CCFLAGS=-g -Wall -std=c++11
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = play
INCLUDE_PATHS = -I include 
LIBRARY_PATHS = -L lib
LINKER_FLAGS = -lSDL2 -lSDL2_image
OBJS = $(BUILD_DIR)/collision.o $(BUILD_DIR)/entity.o $(BUILD_DIR)/game.o $(BUILD_DIR)/player.o $(BUILD_DIR)/texturemanager.o $(BUILD_DIR)/texturemap.o $(BUILD_DIR)/keyboardcontroller.o $(BUILD_DIR)/interactables.o $(BUILD_DIR)/leveleditor.o 

all: debug

run: $(BUILD_DIR)/.dirstamp $(BUILD_DIR)/play runexec
runeditor: $(BUILD_DIR)/.dirstamp $(BUILD_DIR)/play runedit

debug: $(BUILD_DIR)/.dirstamp $(BUILD_DIR)/play

$(BUILD_DIR)/play: $(BUILD_DIR)/game.o $(SRC_DIR)/main.cpp $(OBJS)
	$(CC) $(CCFLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(OBJS)  $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/$(OBJ_NAME)
$(BUILD_DIR)/collision.o: $(BUILD_DIR)/game.o $(SRC_DIR)/collision.h $(SRC_DIR)/collision.cpp
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/collision.cpp
$(BUILD_DIR)/entity.o: $(BUILD_DIR)/game.o $(SRC_DIR)/entity.h $(SRC_DIR)/entity.cpp $(BUILD_DIR)/texturemanager.o $(BUILD_DIR)/collision.o
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/entity.cpp
$(BUILD_DIR)/game.o: $(SRC_DIR)/game.cpp $(SRC_DIR)/game.h 
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/game.cpp
$(BUILD_DIR)/player.o: $(BUILD_DIR)/game.o $(SRC_DIR)/player.h $(SRC_DIR)/player.cpp $(BUILD_DIR)/entity.o $(BUILD_DIR)/texturemanager.o $(SRC_DIR)/animation.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/player.cpp
$(BUILD_DIR)/texturemanager.o: $(BUILD_DIR)/game.o $(SRC_DIR)/texturemanager.h $(SRC_DIR)/texturemanager.cpp
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/texturemanager.cpp
$(BUILD_DIR)/texturemap.o: $(BUILD_DIR)/game.o $(SRC_DIR)/texturemap.h $(SRC_DIR)/texturemap.cpp $(BUILD_DIR)/texturemanager.o
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/texturemap.cpp
$(BUILD_DIR)/keyboardcontroller.o: $(BUILD_DIR)/game.o $(SRC_DIR)/keyboardcontroller.h $(SRC_DIR)/keyboardcontroller.cpp
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/keyboardcontroller.cpp
$(BUILD_DIR)/interactables.o: $(BUILD_DIR)/game.o $(SRC_DIR)/interactables.h $(SRC_DIR)/interactables.cpp $(BUILD_DIR)/entity.o $(BUILD_DIR)/texturemanager.o
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/interactables.cpp
$(BUILD_DIR)/leveleditor.o: $(BUILD_DIR)/game.o $(SRC_DIR)/leveleditor.h $(SRC_DIR)/leveleditor.cpp $(BUILD_DIR)/keyboardcontroller.o
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/leveleditor.cpp

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/.dirstamp:
	mkdir -p $(BUILD_DIR)
	touch $(BUILD_DIR)/.dirstamp

runexec:
	$(BUILD_DIR)/play

runedit:
	$(BUILD_DIR)/play LEVELEDITOR

leaks:
	leaks --atExit -- $(BUILD_DIR)/play