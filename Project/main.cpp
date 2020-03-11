#include "game.h"
#include "player.h"
#include "sdl_wrapper.h"

int 
main(void)
{
  std::unique_ptr<Game>    test = std::make_unique<Game>(640, 480);
  test->init(1);
  test->set_main_player(std::make_shared<Player>(640, 480));
  test->proc_input();
}
