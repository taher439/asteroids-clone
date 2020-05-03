#include "game.h"
#include "player.h"
#include "sdl_wrapper.h"
// #include <SDL_ttf.h>

int 
main(int argc, char *argv[])
{
  int c;
  bool multiplayer = false;

  // if(TTF_Init()==-1) 
  // {
  //     printf("TTF_Init: %s\n", TTF_GetError());
  //     exit(2);
  // }

  if (argc != 1) {
    while( ( c = getopt (argc, argv, "m") ) != -1 ) 
    {
        switch(c)
        {
            case 'm':
#ifdef DEBUG
                std::cout << "[+] 2 player mode selected\n";
#endif
              multiplayer = true;
              break;
        }
    }
  }
  std::unique_ptr<Game>    test = std::make_unique<Game>(640, 480);
  test->init(1, multiplayer);
  //
  test->proc_input();
  return 0;
}
