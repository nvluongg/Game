#include "sprite.h"

void Sprite::init(SDL_Texture* _texture, int frames, const int _clips [][4]) {
    currentFrame = 0;
    int temp = 0;
    texture = _texture;

    SDL_Rect clip;
    for (int i = 0; i < frames; i++) {
            clip.x = _clips[i][0];
            clip.y = _clips[i][1];
            clip.w = _clips[i][2];
            clip.h = _clips[i][3];
            clips.push_back(clip);
        }
}
void Sprite::tick() {
        temp++;
        if( temp % 5 ==0)
        currentFrame = (currentFrame + 1) % clips.size();
}

const SDL_Rect* Sprite::getCurrentClip() const {
    return &(clips[currentFrame]);
}
