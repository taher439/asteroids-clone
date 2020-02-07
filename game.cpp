#include "game.h"
#include "sdl_wrapper.h"
#include "player.h"

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
  for (auto& p: this->players) {
    SDL_DestroyTexture(p->get_tex());
  }
  
  SDL_Quit();
}

void Game::proc_input(void) 
{
  double sprite_angle = 0;
  int x, y;

  while(!this->quit) { 
    while(SDL_PollEvent(&this->ev) != 0) {       
      switch(this->ev.type) {
        case SDL_QUIT:
          this->quit = true;
          break;
        case SDL_MOUSEMOTION:
          SDL_GetMouseState(&x, &y);
          int delta_x = main_player->get_x() - x;
          int delta_y = main_player->get_y() - y;
          sprite_angle = (atan2(delta_y, delta_x) * 180.0000) / PI;
          main_player->set_angle(sprite_angle);
          break;
      }
    }
    SDL_wrapper::rend_clear(this->rend);
    for (auto& p: this->players) {
      SDL_wrapper::rend_copy_ex(this->rend, p);
    }
      

    SDL_RenderPresent(this->rend);
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
  player->set_surf(IMG_Load(path.c_str()));
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
    player->set_tex(SDL_CreateTextureFromSurface(this->rend, player->get_surf()));
    if (player->get_tex() == NULL)
      std::cerr 
           << "Unable to create texture from %s! SDL Error: "
           << path.c_str() << " "
           << SDL_GetError() 
           << std::endl;
    SDL_FreeSurface(player->get_surf());
    this->players.push_back(player);
  }
}

inline SDL_Window *
Game::get_win(void) 
{
  return this->win;
}

inline std::shared_ptr<Player> 
Game::get_main_player(void) 
{
  return this->main_player;
}

inline void
Game::set_main_player(std::shared_ptr<Player> p) 
{
  this->main_player = p;
}
