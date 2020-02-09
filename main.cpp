#include "game.h"
#include "player.h"
#include "sdl_wrapper.h"

int 
main(void)
{
  std::shared_ptr<Player>  p    = std::make_shared<Player>();
  std::unique_ptr<Game>    test = std::make_unique<Game>();
  
  test->init();
  test->set_main_player(p);
  test->proc_input();
}
