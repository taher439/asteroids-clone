#include "player.h"
#include "game.h"
#include "sdl_wrapper.h"

Game::Game() 
{
  this->win = NULL;
  this->surf = NULL;
}

void 
Game::init(void) 
{
  SDL_wrapper::init();
  this->win = SDL_wrapper::creat_win(this->SCREEN_WIDTH, this->SCREEN_HEIGHT);
  this->rend = SDL_wrapper::creat_rend(this->win);
}

void 
Game::kill(void) 
{
  SDL_Quit();
}

void Game::proc_input(void) 
{
  double sprite_angle = 0;
  int x, y;
  bool thrust = false;
  double last_angle = 0;

  while(!this->quit) { 
    while(SDL_PollEvent(&this->ev) != 0) {       
      switch(this->ev.type) {
        case SDL_QUIT:
          this->quit = true;
          break;

        case SDL_MOUSEMOTION:
        {
          SDL_GetMouseState(&x, &y);
          int delta_x = main_player->get_x() - x;
          int delta_y = main_player->get_y() - y;
          sprite_angle = (atan2(delta_y, delta_x) * 180.0000) / PI;
          main_player->set_angle(sprite_angle);
          break;
        }
        case SDL_MOUSEBUTTONDOWN:
          if (this->ev.button.button == SDL_BUTTON_LEFT) {
            thrust = true;
            last_angle = main_player->get_angle();
          }
          break;
      }
    }
    if (thrust)
      main_player->thrust(last_angle);
    SDL_wrapper::rend_clear(this->rend);
    for (auto& p: this->players) {
      SDL_wrapper::rend_copy_ex(this->rend, p);
    }
    SDL_RenderPresent(this->rend.get());
  }
  
  if (this->quit)
    this->kill();
}

void 
Game::load_tex(std::shared_ptr<Player> player, 
               std::string path, 
               std::shared_ptr<SDL_Rect> src, 
               std::shared_ptr<SDL_Rect> dst)
{
  auto tmp_surf = sdl_shared(IMG_Load(path.c_str()));
  player->set_surf(tmp_surf);
  player->set_rect_dst(dst);
  player->set_rect_src(src);
  if(player->get_surf() == NULL)
    std::cerr 
         << "Unable to load image, SDL_image Error " 
         << path.c_str() << " " 
         << IMG_GetError() 
         << std::endl;
  else
  {
    auto tmp_tex = sdl_shared(SDL_CreateTextureFromSurface(
                                this->rend.get(), 
                                player->get_surf().get()));

    player->set_tex(tmp_tex);
    if (player->get_tex() == NULL)
      std::cerr 
           << "Unable to create texture from %s! SDL Error: "
           << path.c_str() << " "
           << SDL_GetError() 
           << std::endl;
    this->players.push_back(player);
  }
}
