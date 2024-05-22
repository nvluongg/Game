#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "defs.h"
#include "sprite.h"
#include "game.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>
#include <sstream>
using namespace std;

struct Scrollbk
{
    SDL_Texture* texture;
    int sc = 0;
    int width, height;

    void setTexture(SDL_Texture* _texture);
    void scroll(int distance) ;

};

struct Graphics{
    SDL_Renderer *renderer;
    SDL_Window *window;
    vector<SDL_Texture*> pic;
    vector<Mix_Music*> music;
    vector<Mix_Chunk*> sound;
    TTF_Font* font;
    Scrollbk bk;

    Graphics(){}

    void loadMedia();
    void free();
    void logErrorAndExit(const char* msg, const char* error);
    void init();

    void prepareScene();
    void presentScene();

    SDL_Texture *loadTexture(const char *filename);
    void renderTexture(SDL_Texture *texture, int x, int y);
    void renderTexture(SDL_Texture *texture, int x, int y,int w,int h);

    void render(const Scrollbk& background);

    Mix_Music* loadMusic(const char* filename);
    void play(Mix_Music* gMusic);

    Mix_Chunk* loadSound(const char* filename);
    void play(Mix_Chunk* gchunk);

    void render(int x, int y, const Sprite& sprite);

    TTF_Font* loadFont(const char* path, int size);
    SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor);

    void quit();
};
#endif
