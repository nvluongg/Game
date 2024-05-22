#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <vector>
#include <SDL.h>
struct Sprite {
    SDL_Texture* texture;
    std::vector<SDL_Rect> clips;
    int currentFrame;
    int temp;

    void init(SDL_Texture* _texture, int frames, const int _clips [][4]);

    void tick();

    const SDL_Rect* getCurrentClip() const;
};
#endif // _SPRITE_H_
