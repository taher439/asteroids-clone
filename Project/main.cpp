#include "game.h"
#include "player.h"
#include "sdl_wrapper.h"

int 
main(void)
{
  std::unique_ptr<Game>    test = std::make_unique<Game>();
  test->init();
  test->set_main_player(std::make_shared<Player>());
  test->proc_input();
}
