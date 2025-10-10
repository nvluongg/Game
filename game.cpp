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
      x-=lanesize;
      if(x<leftlanex)
      x+=lanesize;
}
void Mycar::turnRight()
{
      x+=lanesize;
      if(x>rightlanex)
      x-=lanesize;
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
    car.x=midlane2x;
    car.y=SCREEN_HEIGHT-carsizey;
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
    //scores and speed
    scores=1;
    k=0;
    speed=3;
    // font

}

void Game::prepare()
{   srand(time(NULL));
    graphics.bk.setTexture(graphics.pic[BACKGROUND]);
    set();
    sprite.init(graphics.pic[14],EXPLODE_FRAMES,EXPLODE_CLIPS);
    {
        int tw, th;
        SDL_QueryTexture(graphics.pic[COIN], nullptr, nullptr, &tw, &th);
        const int fw = tw / COIN_FRAMES, fh = th;

        static int COIN_CLIPS[COIN_FRAMES][4];
        for (int i = 0; i < COIN_FRAMES; ++i) {
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
    if(xMouse>140&&xMouse<280&&yMouse>220&&yMouse<332)
    {
            return true;
          }
return false;
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
      SDL_SetTextureAlphaMod(graphics.pic[MY_CAR], isInvisible ? 128 : 255);
      graphics.renderTexture(graphics.pic[ MY_CAR],car.x,car.y);
      graphics.renderEx(coin.x, coin.y, 60, 60, coinSprite, 90.0);
      coinSprite.tick();

      SDL_Texture* coinText = graphics.renderText(renderScore("Coins: ", coinCount),graphics.font, white);
      graphics.renderTexture(coinText, 10, 40);
      SDL_DestroyTexture(coinText);

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

     }
    else if(status==Menu)
    {
      graphics.renderTexture(graphics.pic[MENU],0,0);
      graphics.renderTexture(graphics.pic[UNSELECT_PLAY],150,140);
      graphics.renderTexture(graphics.pic[TITLE],45,90);
      graphics.renderTexture(graphics.pic[UNSELECT_EXIT],150,220);
      if(menuToStart(xMouse,yMouse)) graphics.renderTexture(graphics.pic[PLAY_BUTTON],150,140);
      if(menuToExit(xMouse,yMouse)) graphics.renderTexture(graphics.pic[EXIT_BUTTON],150,220);
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


        invis.move();
        if (checkCollision(invis.x, invis.y - 80, car.x, car.y))
        {
            isInvisible = true;
            invisibleTimer = INVIS_DURATION;
            invis.y = -2500;
            invis.x = lane[rand() % 4];
            graphics.play(graphics.sound[3]);
        }
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
         else if(event.type==SDL_KEYDOWN)
            car.Mycarmove(event);
         else if(event.type==SDL_MOUSEBUTTONDOWN)
         {
             if(status==Menu)
             {
                if(menuToStart(xMouse,yMouse))
                 {
                  graphics.play(graphics.sound[0]);
                  status=Start;
                 }
                else if(menuToExit(xMouse,yMouse))
                    status=QuitGame;
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
