#include "game.h"

int 
main(int argc, char *argv[])
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

  test->load_tex(p, "./ship.png", src.get(), dst.get());
  test->proc_input();
}
