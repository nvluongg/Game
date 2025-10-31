#ifndef _GAME_H
#define _GAME_H
#include "graphics.h"

struct Mycar
{
   int x,y;
   int laneIdx = 2;
   Mycar(){}

   void moveUp();
   void moveDown();
   void turnRight();
   void turnLeft();
   void Mycarmove(SDL_Event &event);
};

struct Othercar
{
   int x,y;
   SDL_Texture* texture;
   Othercar(){}
   void move(int i);
};
struct Shield
{
    int x,y;
    void shieldmove();
};
struct InvisOrb
{
    int x, y;
    void move();
};
struct Coin {
    int x, y;
    void move();
};
struct BossTruck {
    int x, y;
    bool active;
    int laneIdx;
    float vy;
    SDL_Texture* texture;
};


bool checkCollision(int x1,int y1,int x2,int y2);
struct Game
{
    SDL_Event event;
    SDL_Texture* texture[2];

    Mycar car;
    int currentSkin = MY_CAR;
    Othercar ocar[4];
    Sprite sprite;
    Shield shield;
    InvisOrb invis;
    Sprite coinSprite;
    Coin coin;
    int coinCount = 0;
    BossTruck boss;
    int nextBossScore;


    GameStatus status=Menu;

    int xMouse,yMouse;
    int Playerlives=3;
    int scores;
    int highscore;
    int xboom,yboom;
    int shieldCount = 0;
    bool isExplode=false;
    int k=0;
    bool musicStarted=false;
    bool isInvisible = false;
    int invisibleTimer = 0;
//    bool haveShield=false;
    bool delaygame=false;
    int delay=-1;
    bool isDead=false;

    Game(){}
    void free();

    void displayMusic();
    void renderExplode();
    const char* renderScore(const char* a,int b);

    void getMousePos(int &x,int &y);
    bool menuToStart(int x,int y);
    bool menuToExit(int x,int y);
    bool menuToShop(int x,int y);
    bool shopToExit(int x,int y);
    bool shopBuyGreen(int x, int y);
    bool shopBuyBlack(int x, int y);
    bool shopBuyPurple(int x, int y);
    bool shopBuyWhite(int x, int y);
    bool overToPlayAgain(int x,int y);
    bool overToQuit(int x,int y);
    void prepare();
    void set();
    void getCrash();
    void gameOver();
    void render();
    void update();
    void run();
};


#endif
