#include "game.h"

int 
main(int argc, char *argv[])
{
  std::unique_ptr<Game> test(new Game());
  test->init();
  test->proc_input();
}
