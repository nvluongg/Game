
#include "game.h"

Graphics graphics;

int main(int argc, char *argv[])
{
    graphics.init();
    Game game;
    game.prepare();
  while(game.status!=QuitGame)
  {   graphics.prepareScene();

      game.run();

      graphics.presentScene();

      SDL_Delay(15);
  }
  game.free();
return 0;
}
