#include "game.h"
#include "player.h"
#include "sdl_wrapper.h"

int 
main(void)
{
  std::unique_ptr<Game>    test = std::make_unique<Game>();
  test->init();

  std::shared_ptr<Player>   p   = std::make_shared<Player>();
  std::shared_ptr<SDL_Rect> dst = std::make_shared<SDL_Rect>();
  std::shared_ptr<SDL_Rect> src = std::make_shared<SDL_Rect>();

  src->h = 1000;
  src->w = 1000;
  src->x = 0;
  src->y = 0;

  dst->h = 50;
  dst->w = 50;
  dst->x = 640 / 2 - 8;
  dst->y = 480 / 2 - 8;
  test->load_tex(p, "./ship.png", src, dst);
  test->set_main_player(p);
  test->proc_input();
}
