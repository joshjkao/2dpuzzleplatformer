# 2dpuzzleplatformer

This project is built using SDL2, available here: https://github.com/libsdl-org/

Once SDL2 is up and running, compile and run the program with "make run"

This is a very early work in progress of a small game I've been working on in my free time. The basic idea is that the game takes place on multiple windows which the player can freely move between, but their location on screen is always continuous. By cleverly placing walls and obstacles, I could create some interesting puzzles making use of this mechanic. 

However, rather than use an engine I decided to write my own from scratch (minus the SDL2 parts) as I also wanted to improve my familiarity with memory management and OOD and was curious about how graphics worked in C++. A this point I was also looking at C++ and SDL2 to write a separate project as part of my physics curriculum, so I decided to try it out.

The game currently supports player movement and collisons with some simple placeholder animations also by me. A level consists of multiple windows, and each window has their own set of map colliders to interact with and hazards that kill the player. Each level has one "Goal" that currently just outputs a message when it is touched. Level data is currently stored in text files, the format of which is outputted by the simple level editor I created. The instructions for said level editor can be found below. 

In the future, I plan on implementing a way to edit the texturemaps for each level inside the level editor, along with possibly opening an existing file to edit it. From there, I will then need the logic to respawn or start a new level as necessary and potentially save player progress in a similar file system. After that, the next is fine-tuning, designing more levels, and improving the textures and animations. SDL2 also has a sound interface, which I plan on also playing with.

More about the level editor:
The editor can be accessed using "make runedit". Creating windows and colliders can be done by placing marker 1 at the top left corner of the desired rectangle and marker 2 at the bottom right. Once both markers are down, pressing W will create a window and C will create a collider. H and G are for hazards and the goal respectively, however they will always be placed at marker 1 and have fixed dimensions. Note that colliders, hazards, and goals are always attached to the current selected window, which is highlighted in green. By default, this is the last window created and can be changed with the left and right arrow keys. Finally, by default mouse coordinates will be snapped to a 32px32p grid (the size of 1 game tile), but this can be toggled on or off with S. Pressing ESCAPE will close the editor and save the file as "newlevel.txt".
This tool is very rudimentary and incomplete, although I plan on implementing deleting or moving objects as well as setting the texturemap for the background (currently the background is always black).
