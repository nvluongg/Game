#ifndef _DEFS__H
#define _DEFS__H

#define  SCREEN_WIDTH 500
#define  SCREEN_HEIGHT  680
#define WINDOW_TITLE "GAME OF LUONG"

#define carspeed 15

#define lanesize  67
#define leftlanex  120
#define midlane1x  leftlanex + lanesize
#define midlane2x  midlane1x + lanesize
#define rightlanex  midlane2x + lanesize
const int lane[4] = { leftlanex, midlane1x, midlane2x, rightlanex };

#define carsizex 70
#define carsizey 120

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
#define INVIS_ORB 18
#define TRUCK 20
#define SHOP_BUTTON 21
#define UNSELECT_SHOP 22
#define SHOP_BG 23
#define CAR_BLACK 24
#define CAR_PURPLE 25
#define CAR_WHITE 26
#define BUY_BUTTON 27
#define UNSELECT_BUY 28
#define PAUSE_BUTTON 29
#define UNSELECT_PAUSE 30

//TRUCK
#define BOSS_W  (lanesize * 2 - 10)
#define BOSS_H  220
#define BOSS_SPEED_ADD 0.3f
#define BOSS_SCORE_STEP 200
//INVIS
#define SND_INVIS 3
#define INVIS_DURATION 200
//COIN
#define COIN 19
#define COIN_FRAMES 7
//3Shield
#define MAX_SHIELDS 3
#define SHIELD_ICON_W 32
#define SHIELD_ICON_H 22
#define SHIELD_ICON_X0 80
#define SHIELD_ICON_Y 8
#define SHIELD_ICON_DX 24
//map
#define TEX_ROAD9_BG       5
#define TEX_EXIT_BUTTON    10
#define TEX_MAP_BUTTON         31
#define TEX_MAP_BUTTON_UNSEL   32
#define TEX_MAP_WINDOW_BG      33
#define TEX_SELECT_BUTTON      34

#define TEX_MAP_CITY_THUMB     35

#define TEX_MAP_DESERT_BG      36
#define TEX_MAP_DESERT_THUMB   37
#define TEX_MAP_SEA_BG         38
#define TEX_MAP_SEA_THUMB      39

#define TEX_MAP_SNOW_BG        40
#define TEX_MAP_SNOW_THUMB     41
#define TEX_SELECT_BUTTON_UNSEL  42
#define POISON_ORB 43
#define CURE_ORB   44


#define MAP_GRID_X1   40
#define MAP_GRID_Y1   150
#define MAP_GRID_X2   270
#define MAP_GRID_Y2   380
#define MAP_THUMB_W   180
#define MAP_THUMB_H   120
#define MAP_BTN_W     120
#define MAP_BTN_H     45
#define MAP_BTN_DY    10
#define MAP_EXIT_X    190
#define MAP_EXIT_Y    600
#define MAP_EXIT_W    120
#define MAP_EXIT_H    45


enum MapId {
    MAP_CITY = 0,
    MAP_DESERT = 1,
    MAP_SEA = 2,
    MAP_SNOW = 3
};


enum GameStatus{
      Menu,
      Start,
      GameOver,
      Shop,
      Map,
      PauseGame,
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
