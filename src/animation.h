#ifndef ANIMATION_H
#define ANIMATION_H

struct Animation {
    int a_index;
    int a_frames;
    int a_speed;

    Animation() {
        
    }
    Animation(int index_, int frames_, int speed_):
    a_index(index_), a_frames(frames_), a_speed(speed_)
    {
        std::cout << "loaded animation" << std::endl;
    }

};

#endif