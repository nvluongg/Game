#include "game.h"
using namespace std;
extern Graphics graphics;
float speed;
void Mycar::moveUp()
{
     y-=carspeed;
     if(y<0)
     y+=carspeed;
}
void Mycar::moveDown()
{
    y+=carspeed;
    if(y>SCREEN_HEIGHT-100)
    y-=carspeed;
}
void Mycar::turnLeft()
{
    if (laneIdx > 0) {
        laneIdx--;
        x = lane[laneIdx];
    }
}
void Mycar::turnRight()
{
      if (laneIdx < 3) {
        laneIdx++;
        x = lane[laneIdx];
    }
}
void Mycar::Mycarmove(SDL_Event &event)
{
             switch(event.key.keysym.sym)
          {
               case SDLK_UP:
                  moveUp();
                  break;
               case SDLK_DOWN:
                  moveDown();
                  break;
               case SDLK_RIGHT:
                 turnRight();
                  break;
               case SDLK_LEFT:
                 turnLeft();
                  break;
          }
}
void Othercar::move(int i)
{       y+=speed;
        if(y>SCREEN_HEIGHT)
        {   y=-(200+rand()%101);
            if(i%2==0)
             x=lane[rand()%2+2];
           else x=lane[rand()%2];
        }
}
void Shield::shieldmove()
{
    y+=speed;
    if(y>SCREEN_HEIGHT)
    {
        y=0;
        x=lane[rand()%4];
    }
}
void InvisOrb::move() {
    y += speed;
    if (y > SCREEN_HEIGHT) {
        y = 0;
        x = lane[rand()%4];
    }
}

void Coin::move() {
    y += speed;
    if (y > SCREEN_HEIGHT) {
        y = -(300 + rand() % 700);
        int lanes[4] = { leftlanex, midlane1x, midlane2x, rightlanex };
        x = lanes[rand() % 4];
    }
}

void PoisonOrb::move() {
    y += speed;
    if (y > SCREEN_HEIGHT) {
        y = - (rand() % 1000 + 500);
        x = lane[rand() % 4];
    }
}

void CureOrb::move() {
    y += speed;
    if (y > SCREEN_HEIGHT) {
        y = - (rand() % 1000 + 800);
        x = lane[rand() % 4];
    }
}


int Game::mapBgTexIndex() const {
    switch (currentMap) {
        case MAP_CITY:   return TEX_ROAD9_BG;
        case MAP_DESERT: return TEX_MAP_DESERT_BG;
        case MAP_SEA:    return TEX_MAP_SEA_BG;
        case MAP_SNOW:   return TEX_MAP_SNOW_BG;
        default:         return TEX_ROAD9_BG;
    }
}


void Game::set()
{
    ocar[0].x=lane[rand()%2+2];
    ocar[0].texture=graphics.pic[CAR_1];
    ocar[0].y=-200;
    ocar[1].texture=graphics.pic[CAR_2];
    ocar[2].texture=graphics.pic[CAR_3];
    ocar[3].texture=graphics.pic[CAR_4];
    for(int i=1;i<4;i++)
    {
      if(i%2==0)
        ocar[i].x=lane[rand()%2+2];
      else ocar[i].x=lane[rand()%2];
      ocar[i].y=ocar[i-1].y-(rand()%101+400);
    }
    //my car
    car.laneIdx = 2;
    car.x = lane[car.laneIdx];
    car.y = SCREEN_HEIGHT - carsizey;
    //shield
    shieldCount = 0;
    shield.x=lane[rand()%4];
    shield.y=-2000;
    // invisible orb
    invis.x = lane[rand()%4];
    invis.y = -200;
    //coin
    int lanes[4] = { leftlanex, midlane1x, midlane2x, rightlanex };
    coin.x = lanes[rand() % 4];
    coin.y = -(500 + rand() % 1000);
    coinCount = 0;
    //truck
    boss.active = false;
    boss.texture = graphics.pic[TRUCK];
    boss.x = lane[rand()%4];
    boss.y = -BOSS_H;
    nextBossScore = 200;
    //poison and cure
    poison.x = lane[rand()%4];
    poison.y = -1000;
    cure.x = lane[rand()%4];
    cure.y = -2000;
    isPoisoned = false;
    poisonTimer = 0;


    //scores and speed
    scores=1;
    k=0;
    speed=3;
    // pause
    Playerlives = 3;
    isDead = false;
    isExplode = false;

    isInvisible = false;
    invisibleTimer = 0;

    delaygame = false;
    delay = -1;

    boss.active = false;
}

void Game::prepare()
    {
    srand(time(NULL));
    graphics.bk.setTexture(graphics.pic[ mapBgTexIndex() ]);
    set();
    sprite.init(graphics.pic[14],EXPLODE_FRAMES,EXPLODE_CLIPS);
        {
        int tw, th;
        SDL_QueryTexture(graphics.pic[COIN], nullptr, nullptr, &tw, &th);
        const int fw = tw / COIN_FRAMES, fh = th;

        static int COIN_CLIPS[COIN_FRAMES][4];
        for (int i = 0; i < COIN_FRAMES; ++i)
            {
            COIN_CLIPS[i][0] = i * fw; // x
            COIN_CLIPS[i][1] = 0;      // y
            COIN_CLIPS[i][2] = fw;     // w
            COIN_CLIPS[i][3] = fh;     // h
            }
        coinSprite.init(graphics.pic[COIN], COIN_FRAMES, COIN_CLIPS);
        }
    }

bool checkCollision(int x1,int y1,int x2,int y2)
{
        if(x1==x2){
            if((y1+carsizey>y2&&y2>y1)||(y2+carsizey>y1&&y1>y2))
               return true;
        }
return false;
}

inline bool AABB(int x1, int y1, int w1, int h1,int x2, int y2, int w2, int h2)
{
    return !(x1 + w1 <= x2 || x2 + w2 <= x1 ||
             y1 + h1 <= y2 || y2 + h2 <= y1);
}

static void calcBossRectForTwoLanes(int i, int& outX, int& outW) {
    outX = lane[i];
    outW = lanesize * 2 - 10;
}

void Game::getMousePos(int &x,int &y)
{
    SDL_GetMouseState(&x,&y);
}

bool Game::menuToStart(int xMouse,int yMouse)
{
    if(xMouse>140&&xMouse<380&&yMouse>120&&yMouse<230)
    {
            return true;
          }
return false;
}
bool Game::menuToExit(int x,int y)
{
    if(xMouse>140&&xMouse<280&&yMouse>220&&yMouse<330)
    {
            return true;
          }
return false;
}
bool Game::menuToShop(int x,int y)
{
    if(xMouse>140&&xMouse<280&&yMouse>320&&yMouse<390)
    {
            return true;
          }
return false;
}
bool Game::shopToExit(int x, int y)
{
    if (x > 150 && x < 290 && y > 580 && y < 640)
        return true;
    return false;
}
bool Game::shopBuyGreen(int mx, int my) {
    int bx = 60;
    int by = 260;
    int bw = 140;
    int bh = 50;
    return (mx > bx && mx < bx + bw &&
            my > by && my < by + bh);
}

bool Game::shopBuyBlack(int mx, int my) {
    int bx = 280;
    int by = 260;
    int bw = 140;
    int bh = 50;
    return (mx > bx && mx < bx + bw &&
            my > by && my < by + bh);
}

bool Game::shopBuyPurple(int mx, int my) {
    int bx = 60;
    int by = 470;
    int bw = 140;
    int bh = 50;
    return (mx > bx && mx < bx + bw &&
            my > by && my < by + bh);
}

bool Game::shopBuyWhite(int mx, int my) {
    int bx = 280;
    int by = 520;
    int bw = 140;
    int bh = 50;
    return (mx > bx && mx < bx + bw &&
            my > by && my < by + bh);
}
bool Game::onPauseBtn(int mx, int my) {
    const int w = 120, h = 45;
    const int x = SCREEN_WIDTH - w - 10;
    const int y = 40;
    return (mx > x && mx < x + w && my > y && my < y + h);
}

bool Game::onPauseExitBtn(int mx, int my) {
    const int w = 120, h = 45;
    const int x = SCREEN_WIDTH - w - 10;
    const int y = 40 + 45 + 10;
    return (mx > x && mx < x + w && my > y && my < y + h);
}

bool Game::menuToMap(int mx, int my) {
    int x = 150, y = 380, w = 200, h = 73;
    return (mx > x && mx < x + w && my > y && my < y + h);
}

bool Game::mapToExit(int mx, int my) {
    return (mx > MAP_EXIT_X && mx < MAP_EXIT_X + MAP_EXIT_W &&
            my > MAP_EXIT_Y && my < MAP_EXIT_Y + MAP_EXIT_H);
}

bool Game::mapSelectHover(int mx, int my, int cellIndex) {
    int col = (cellIndex % 2);
    int row = (cellIndex / 2);
    int X = (col == 0) ? MAP_GRID_X1 : MAP_GRID_X2;
    int Y = (row == 0) ? MAP_GRID_Y1 : MAP_GRID_Y2;

    bool inThumb =
        (mx > X && mx < X + MAP_THUMB_W &&
         my > Y && my < Y + MAP_THUMB_H);

    int bx = X + (MAP_THUMB_W - MAP_BTN_W) / 2;
    int by = Y + MAP_THUMB_H + MAP_BTN_DY;
    bool inBtn =
        (mx > bx && mx < bx + MAP_BTN_W &&
         my > by && my < by + MAP_BTN_H);

    return inThumb || inBtn;
}

void Game::renderMapScreen(int mx, int my) {

    graphics.renderTexture(graphics.pic[TEX_MAP_WINDOW_BG], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    struct Cell { int texThumb; const char* name; };
    Cell cells[4] = {
        { TEX_MAP_CITY_THUMB,   "CITY"  },
        { TEX_MAP_DESERT_THUMB, "DESERT"},
        { TEX_MAP_SEA_THUMB,    "SEA"   },
        { TEX_MAP_SNOW_THUMB,   "SNOW"  }
    };

    for (int i = 0; i < 4; ++i) {
        int col = (i % 2);
        int row = (i / 2);
        int X = (col == 0) ? MAP_GRID_X1 : MAP_GRID_X2;
        int Y = (row == 0) ? MAP_GRID_Y1 : MAP_GRID_Y2;

        graphics.renderTexture(graphics.pic[cells[i].texThumb], X, Y, MAP_THUMB_W, MAP_THUMB_H);

        int bx = X + (MAP_THUMB_W - MAP_BTN_W) / 2;
        int by = Y + MAP_THUMB_H + MAP_BTN_DY;
        bool hov = mapSelectHover(mx, my, i);
        int tex = hov ? TEX_SELECT_BUTTON : TEX_SELECT_BUTTON_UNSEL;
        graphics.renderTexture(graphics.pic[tex], bx, by, MAP_BTN_W, MAP_BTN_H);

        SDL_Texture* t = graphics.renderText(cells[i].name, graphics.font, white);
        graphics.renderTexture(t, X + 8, Y + 8);
        SDL_DestroyTexture(t);
    }

    bool hovExit = mapToExit(mx, my);
    graphics.renderTexture(graphics.pic[ hovExit ? TEX_EXIT_BUTTON : UNSELECT_EXIT ],
                           MAP_EXIT_X, MAP_EXIT_Y, MAP_EXIT_W, MAP_EXIT_H);
}


bool Game::overToPlayAgain(int x,int y)
{
     if(xMouse>187&&xMouse<240&&yMouse>431&&yMouse<442)
    {
            return true;
          }
return false;
}
bool Game::overToQuit(int x,int y)
{
     if(xMouse>267&&xMouse<341&&yMouse>431&&yMouse<442)
    {
            return true;
          }
return false;
}

const char* Game::renderScore(const char* s,int a)
{
    stringstream ss;
    ss << s << a;
    string b=ss.str();
    const char* result = b.c_str();
return result;

}
void Game::renderExplode()
{
   if(isExplode)
     {
       graphics.render(xboom,yboom,sprite);
       sprite.tick();
        if(sprite.currentFrame==EXPLODE_FRAMES-1)
        {     sprite.currentFrame=0;
               isExplode=false;
        }
     }
}
void Game::displayMusic()
{
    if(status==Menu)
      graphics.play(graphics.music[0]);
    else if(status==Start)
        if(!musicStarted)
        {
            Mix_HaltMusic();
            graphics.play(graphics.music[1]);
            musicStarted = true;
        }
}
void Game::gameOver()
{
     if(delaygame)
        {
         delay=48;
         ifstream file("highscore.txt");
                file >> highscore;
                file.close();
                if(scores>highscore)
                 {
                    highscore=scores;
                    ofstream files("highscore.txt");
                    files << highscore;
                    files.close();
                 }

         delaygame=false;
        }
        if(delay>0)
          delay--;
        if(delay==0)
        {
          status=GameOver;
          graphics.play(graphics.sound[1]);
          delay=-1;
        }
}
void Game::render()
{   getMousePos(xMouse,yMouse);

    if(status==Start)
    { graphics.bk.scroll(speed-1);
      graphics.render(graphics.bk);
      graphics.renderTexture(graphics.pic[INVIS_ORB], invis.x, invis.y, 60, 60);
      graphics.renderTexture(graphics.pic[SHIELD],shield.x,shield.y);
      SDL_SetTextureAlphaMod(graphics.pic[currentSkin], isInvisible ? 128 : 255);
      graphics.renderTexture(graphics.pic[currentSkin], car.x, car.y, carsizex, carsizey);
      graphics.renderEx(coin.x, coin.y, 60, 60, coinSprite, 90.0);
      coinSprite.tick();

      SDL_Texture* coinText = graphics.renderText(renderScore("Coins: ", coinCount),graphics.font, white);
      graphics.renderTexture(coinText, 10, 40);
      SDL_DestroyTexture(coinText);

    if (boss.active)
    {
    graphics.renderTexture(graphics.pic[TRUCK],boss.x, boss.y, boss.w, boss.h);
    }


      for(int i=0;i<4;i++)
    {
        graphics.renderTexture(ocar[i].texture,ocar[i].x,ocar[i].y);
    }
      for(int i=0;i<Playerlives;i++)
    {
        graphics.renderTexture(graphics.pic[HEART],xHeart[i],yHeart);
    }    texture[0]=graphics.renderText(renderScore("Scores: ",scores),graphics.font,white);
         graphics.renderTexture(texture[0],325,10);

        renderExplode();
        for (int i = 0; i < shieldCount; ++i) {
        graphics.renderTexture(
        graphics.pic[SHIELD],
        SHIELD_ICON_X0 + i * SHIELD_ICON_DX,
        SHIELD_ICON_Y,
        SHIELD_ICON_W, SHIELD_ICON_H
        );
    }
        const int pauseW = 120, pauseH = 45;
        const int pauseX = SCREEN_WIDTH - pauseW - 10; // 370
        const int pauseY = 40;
        const bool hoverPause = onPauseBtn(xMouse, yMouse);
        graphics.renderTexture(
            graphics.pic[ hoverPause ? PAUSE_BUTTON : UNSELECT_PAUSE ],
            pauseX, pauseY, pauseW, pauseH
        );

        if (currentMap == MAP_DESERT) {
            graphics.renderTexture(graphics.pic[POISON_ORB], poison.x, poison.y, 60, 60);
            graphics.renderTexture(graphics.pic[CURE_ORB], cure.x, cure.y, 60, 60);
        }
        if (isPoisoned) {
            int barW = 20;
            int barH = 100;
            int barX = SCREEN_WIDTH - 40;
            int barY = SCREEN_HEIGHT - 150;

            SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 255);
            SDL_Rect bg = { barX, barY, barW, barH };
            SDL_RenderFillRect(graphics.renderer, &bg);

            float ratio = (float)poisonTimer / POISON_DURATION;
            int fillH = (int)(barH * ratio);
            SDL_SetRenderDrawColor(graphics.renderer, 0, 255, 0, 255);
            SDL_Rect fill = { barX, barY + (barH - fillH), barW, fillH };
            SDL_RenderFillRect(graphics.renderer, &fill);
        }
}
    else if(status==Menu)
    {
      graphics.renderTexture(graphics.pic[MENU],0,0);
      graphics.renderTexture(graphics.pic[UNSELECT_PLAY],150,140);
      graphics.renderTexture(graphics.pic[TITLE],45,90);
      graphics.renderTexture(graphics.pic[UNSELECT_EXIT],150,220);
      graphics.renderTexture(graphics.pic[UNSELECT_SHOP],150,300, 200, 73);
      if(menuToStart(xMouse,yMouse)) graphics.renderTexture(graphics.pic[PLAY_BUTTON],150,140);
      if(menuToExit(xMouse,yMouse)) graphics.renderTexture(graphics.pic[EXIT_BUTTON],150,220);
      if(menuToShop(xMouse,yMouse)) graphics.renderTexture(graphics.pic[SHOP_BUTTON],150,300, 200, 73);
      bool hov = menuToMap(xMouse, yMouse);
      graphics.renderTexture(graphics.pic[ hov ? TEX_MAP_BUTTON : TEX_MAP_BUTTON_UNSEL ],150, 380, 200, 73);
    }
    else if(status==GameOver)
    {
         graphics.renderTexture(graphics.pic[GAME_OVER],40 ,0);
         if(overToPlayAgain(xMouse,yMouse))
            graphics.renderTexture(graphics.pic[YES],156,410);
         if(overToQuit(xMouse,yMouse))
            graphics.renderTexture(graphics.pic[NO],233,409,130,60);
            graphics.renderTexture(texture[0],70,30);
            texture[1]=graphics.renderText(renderScore("High scores: ",highscore),graphics.font,white);
            graphics.renderTexture(texture[1],70,50);
   }
   else if(status == Shop)
    {
        graphics.renderTexture(graphics.pic[SHOP_BG], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        const int carW = 100;
        const int carH = 150;

        const int xGreen  = 60;   const int yGreen  = 120;  // MY_CAR
        const int xBlack  = 280;  const int yBlack  = 120;  // CAR_BLACK
        const int xPurple = 60;   const int yPurple = 320;  // CAR_PURPLE
        const int xWhite  = 280;  const int yWhite  = 320;  // CAR_WHITE

        graphics.renderTexture(graphics.pic[MY_CAR],      xGreen,  yGreen,  carW, carH);
        graphics.renderTexture(graphics.pic[CAR_BLACK],   xBlack,  yBlack,  carW, carH);
        graphics.renderTexture(graphics.pic[CAR_PURPLE],  xPurple, yPurple, carW, carH);
        graphics.renderTexture(graphics.pic[CAR_WHITE],   xWhite,  yWhite,  carW, carH);

        SDL_Texture* t1 = graphics.renderText("0VND", graphics.font, white);
        graphics.renderTexture(t1,  90, 250);
        SDL_DestroyTexture(t1);

        SDL_Texture* t2 = graphics.renderText("0VND", graphics.font, white);
        graphics.renderTexture(t2, 310, 250);
        SDL_DestroyTexture(t2);

        SDL_Texture* t3 = graphics.renderText("0VND", graphics.font, white);
        graphics.renderTexture(t3,  90, 450);
        SDL_DestroyTexture(t3);

        SDL_Texture* t4 = graphics.renderText("0VND", graphics.font, white);
        graphics.renderTexture(t4, 310, 450);
        SDL_DestroyTexture(t4);

        const int buyW = 110;
        const int buyH = 40;

        int buyGreenX = 60;
        int buyGreenY = 270;
        bool hoverGreen =
            (xMouse > buyGreenX && xMouse < buyGreenX + buyW &&
             yMouse > buyGreenY && yMouse < buyGreenY + buyH);
        graphics.renderTexture(
            graphics.pic[ hoverGreen ? BUY_BUTTON : UNSELECT_BUY ],
            buyGreenX, buyGreenY, buyW, buyH
        );

        int buyBlackX = 280;
        int buyBlackY = 270;
        bool hoverBlack =
            (xMouse > buyBlackX && xMouse < buyBlackX + buyW &&
             yMouse > buyBlackY && yMouse < buyBlackY + buyH);
        graphics.renderTexture(
            graphics.pic[ hoverBlack ? BUY_BUTTON : UNSELECT_BUY ],
            buyBlackX, buyBlackY, buyW, buyH
        );

        int buyPurpleX = 60;
        int buyPurpleY = 470;
        bool hoverPurple =
            (xMouse > buyPurpleX && xMouse < buyPurpleX + buyW &&
             yMouse > buyPurpleY && yMouse < buyPurpleY + buyH);
        graphics.renderTexture(
            graphics.pic[ hoverPurple ? BUY_BUTTON : UNSELECT_BUY ],
            buyPurpleX, buyPurpleY, buyW, buyH
        );

        int buyWhiteX = 280;
        int buyWhiteY = 470;
        bool hoverWhite =
            (xMouse > buyWhiteX && xMouse < buyWhiteX + buyW &&
             yMouse > buyWhiteY && yMouse < buyWhiteY + buyH);
        graphics.renderTexture(
            graphics.pic[ hoverWhite ? BUY_BUTTON : UNSELECT_BUY ],
            buyWhiteX, buyWhiteY, buyW, buyH
        );


        graphics.renderTexture(graphics.pic[UNSELECT_EXIT], 150, 580);
        if (shopToExit(xMouse, yMouse))
        graphics.renderTexture(graphics.pic[EXIT_BUTTON], 150, 580);
    }
    else if (status == PauseGame)
    {
    graphics.renderTexture(graphics.pic[MENU], 0, 0);

    SDL_Texture* t0 = graphics.renderText("GAME PAUSED", graphics.font, white);
    graphics.renderTexture(t0, 150, 200);
    SDL_DestroyTexture(t0);

    SDL_Texture* t1 = graphics.renderText("Press P/ESC or click button to resume", graphics.font, white);
    graphics.renderTexture(t1, 60, 240);
    SDL_DestroyTexture(t1);

    const int pauseW = 120, pauseH = 45;
    const int pauseX = SCREEN_WIDTH - pauseW - 10;
    const int pauseY = 40;
    const bool hoverPause = onPauseBtn(xMouse, yMouse);
    graphics.renderTexture(graphics.pic[ hoverPause ? PAUSE_BUTTON : UNSELECT_PAUSE ],pauseX, pauseY, pauseW, pauseH);

    const int w = 120, h = 45;
    const int x = SCREEN_WIDTH - w - 10;
    const int y = 40 + 45 + 10;
    const bool hovExit = onPauseExitBtn(xMouse, yMouse);
    graphics.renderTexture(graphics.pic[ hovExit ? TEX_EXIT_BUTTON : UNSELECT_EXIT ],x, y, w, h);
    }
    else if (status == Map)
    {
    renderMapScreen(xMouse, yMouse);
    }
}
void Game::update()
{
    if (status == Start)
    {
        if (isInvisible) {
            if (--invisibleTimer <= 0) isInvisible = false;
        }

        for (int i = 0; i < 4; i++)
        {
            ocar[i].move(i);
            if (!isInvisible && checkCollision(car.x, car.y, ocar[i].x, ocar[i].y))
            {
                xboom = ocar[i].x;
                yboom = ocar[i].y;
                ocar[i].y = -600;

                if (shieldCount > 0) {
                    --shieldCount;
                    shield.x = lane[rand() % 4];
                } else {
                    Playerlives--;
                }

                isExplode = true;
                graphics.play(graphics.sound[2]);

                if (Playerlives < 0) {
                    delaygame = true;
                    isDead = true;
                }
            }

            for (int j = i + 1; j < 4; j++)
                if (checkCollision(ocar[i].x, ocar[i].y + 50, ocar[j].x, ocar[j].y))
                    ocar[j].y -= 400;
        }

//shield
        shield.shieldmove();
        if (checkCollision(shield.x, shield.y - 80, car.x, car.y))
        {
            if (shieldCount < MAX_SHIELDS) {
                ++shieldCount;
            }
            shield.y = -2500;
            shield.x = lane[rand() % 4];
            graphics.play(graphics.sound[3]);
        }

//invisible
        invis.move();
        if (checkCollision(invis.x, invis.y - 80, car.x, car.y))
        {
            isInvisible = true;
            invisibleTimer = INVIS_DURATION;
            invis.y = -2500;
            invis.x = lane[rand() % 4];
            graphics.play(graphics.sound[3]);
        }
//coin
        coin.move();
        if (checkCollision(coin.x, coin.y - 80, car.x, car.y)) {
            ++coinCount;
            graphics.play(graphics.sound[3]);

            coin.y = -(500 + rand() % 1000);
            coin.x = lane[rand() % 4];

            if (coinCount >= 3) {
                scores += 100;
                coinCount = 0;
            }
        }
//truck
    if (!boss.active && scores >= nextBossScore)
    {
    boss.active = true;

    int i = rand() % 3;
    int bw;
    calcBossRectForTwoLanes(i, boss.x, bw);

    boss.y  = -BOSS_H;
    boss.vy = speed + BOSS_SPEED_ADD;
    boss.w  = bw;
    boss.h  = BOSS_H;
    }

    if (boss.active)
    {
        boss.y += boss.vy;

        for (int i = 0; i < 4; ++i) {
            if (AABB(ocar[i].x, ocar[i].y + 50, lanesize, carsizey,
                     boss.x, boss.y, BOSS_W, BOSS_H)) {
                ocar[i].y -= 400;
            }
        }

        if (!isInvisible && AABB(car.x, car.y, carsizex, carsizey,boss.x, boss.y, boss.w, boss.h))
        {
            xboom = boss.x;
            yboom = boss.y;
            isExplode = true;
            graphics.play(graphics.sound[2]);

            if (shieldCount > 0) --shieldCount;
            else Playerlives -= 2;

            boss.active = false;
            nextBossScore += BOSS_SCORE_STEP;

            if (Playerlives < 0) {
                delaygame = true;
                isDead = true;
            }
        }


        if (boss.y > SCREEN_HEIGHT) {
            boss.active = false;
            nextBossScore += BOSS_SCORE_STEP;
        }
    }

//poison and cure
    if (currentMap == MAP_DESERT) {
        poison.move();
        if (!isPoisoned && checkCollision(poison.x, poison.y - 80, car.x, car.y)) {
            isPoisoned = true;
            poisonTimer = POISON_DURATION;
            poison.y = -2000;
            graphics.play(graphics.sound[3]);
        }

        cure.move();
        if (isPoisoned && checkCollision(cure.x, cure.y - 80, car.x, car.y)) {
            isPoisoned = false;
            poisonTimer = 0;
            cure.y = -2500;
            graphics.play(graphics.sound[3]);
        }

        if (isPoisoned) {
            poisonTimer--;
            if (poisonTimer <= 0) {
                isPoisoned = false;
                Playerlives--;
                graphics.play(graphics.sound[2]);
            }
        }
    }



//dead
        if (!isDead)
        {
            k++;
            if (k % 15 == 0) scores++;
            if (k % 400 == 0) speed += 0.5;
        }

        gameOver();
    }
}

void Game::run()
{

     while(SDL_PollEvent (&event))
     {
         if(event.type==SDL_QUIT)
            status=QuitGame;
         else if (event.type == SDL_KEYDOWN)
            {
                if (status == Start) {
                    if (event.key.keysym.sym == SDLK_p || event.key.keysym.sym == SDLK_ESCAPE) {
                        status = PauseGame;
                    } else {
                        car.Mycarmove(event);
                    }
                }
                else if (status == PauseGame) {
                    if (event.key.keysym.sym == SDLK_p || event.key.keysym.sym == SDLK_ESCAPE) {
                        status = Start;
                    }
                }
            }

         else if(event.type==SDL_MOUSEBUTTONDOWN)
         {
             if(status==Menu)
             {
                if(menuToStart(xMouse,yMouse))
                 {
                    graphics.play(graphics.sound[0]);
                    status=Start;
                 }
                 //
                 else if(menuToShop(xMouse,yMouse))
                 {
                    status=Shop;
                 }
                 else if (menuToMap(xMouse, yMouse)) {
                    status = Map;
                 }
                 //
                else if(menuToExit(xMouse,yMouse))
                    status=QuitGame;
             }
             else if(status==Start) {
                if (onPauseBtn(xMouse, yMouse)) {
                    status = PauseGame;
                }
            }
            else if (status == PauseGame) {
                if (onPauseBtn(xMouse, yMouse)) {
                    status = Start;
                } else if (onPauseExitBtn(xMouse, yMouse)) {
                    set();
                    Playerlives = 3;
                    isDead = false;
                    isExplode = false;

                    isInvisible = false;
                    invisibleTimer = 0;

                    delaygame = false;
                    delay = -1;

                    status = Menu;
                    Mix_HaltMusic();
                    musicStarted = false;
                }
            }

             else if(status==Shop)
             {
                 if (shopBuyGreen(xMouse, yMouse)) {
                    currentSkin = MY_CAR;
                }
                else if (shopBuyBlack(xMouse, yMouse)) {
                    currentSkin = CAR_BLACK;
                }
                else if (shopBuyPurple(xMouse, yMouse)) {
                    currentSkin = CAR_PURPLE;
                }
                else if (shopBuyWhite(xMouse, yMouse)) {
                    currentSkin = CAR_WHITE;
                }
                 if (shopToExit(xMouse, yMouse))
                 {
                     status = Menu;
                 }
             }
             else if (status == Map)
            {
                for (int i = 0; i < 4; ++i) {
                    if (mapSelectHover(xMouse, yMouse, i)) {
                        currentMap = (MapId)i;

                        graphics.bk.setTexture(graphics.pic[ mapBgTexIndex() ]);

                        status = Menu;
                        break;
                    }
                }
                if (mapToExit(xMouse, yMouse)) {
                    status = Menu;
                }
            }

             else if((status==GameOver))
             {
                 if(overToPlayAgain(xMouse,yMouse))
             {
                set();
                status=Start;
                Playerlives=3;
                isDead=false;
             }   else if(overToQuit(xMouse,yMouse))
                    status=QuitGame;
             }
         }
     }


        update();

        render();

        displayMusic();

}
void Game::free()
{
    graphics.quit();
    for(int i=0;i<2;i++)
        SDL_DestroyTexture(texture[i]);
}
