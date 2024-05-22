#ifndef _DEFS__H
#define _DEFS__H

#define  SCREEN_WIDTH 500
#define  SCREEN_HEIGHT  680
#define WINDOW_TITLE "GAME OF LUONG"

#define carspeed 15

#define lanesize  59
#define leftlanex  100
#define midlane1x  leftlanex + lanesize
#define midlane2x  midlane1x + lanesize
#define rightlanex  midlane2x + lanesize

#define carsizex 50
#define carsizey 100

const int lane[4]={leftlanex,midlane1x,midlane2x,rightlanex};

const int xHeart[3]={10,35,60};
#define yHeart 10

#define MY_CAR 0
#define CAR_1 1
#define CAR_2 2
#define CAR_3 3
#define CAR_4 4
#define BACKGROUND 5
#define MENU 6
#define HEART 7
#define PLAY_BUTTON 8
#define UNSELECT_PLAY 9
#define EXIT_BUTTON 10
#define UNSELECT_EXIT 11
#define TITLE 12
#define GAME_OVER 13
#define EXPLODE 14
#define SHIELD 15
#define YES 16
#define NO 17

enum GameStatus{
      Menu,
      Start,
      GameOver,
      QuitGame
};

const int EXPLODE_CLIPS[][4] = {
    {0,0,60,60},
    {60,0,60,60},
    {120,0,60,60},
    {180,0,60,60},
    {240,0,60,60},
    {300,0,60,60},
    {360,0,60,60},
    {420,0,60,60},
    {480,0,60,60},
    {540,0,60,60},
    {600,0,60,60},
    {660,0,60,60},


};

const int EXPLODE_FRAMES = sizeof(EXPLODE_CLIPS) / sizeof(int) / 4;

const SDL_Color white = {255,255,255};

#endif
