#include "game.h"

int 
main(int argc, char * argv[])
{
  std::unique_ptr<Game> test(new Game());
  test->init();

  bool quit = false;
  SDL_Event ev;

  while(!quit) {
    
    while(SDL_PollEvent(&ev) != 0) {
          
      if(ev.type == SDL_QUIT)
        quit = true;
    }

    SDL_UpdateWindowSurface(test->get_win());
  }
}
