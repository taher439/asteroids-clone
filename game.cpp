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
}

void 
Game::kill(void) 
{
  for (auto& p: this->players) {
    SDL_DestroyTexture(p->get_tex());
  }
  SDL_DestroyRenderer(this->rend);
  SDL_DestroyWindow(this->win);
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
    SDL_RenderClear(this->rend);
    for (auto& p: this->players) {
      SDL_RenderCopyEx(this->rend, 
                     p->get_tex(), 
                     p->get_rect_src(), 
                     p->get_rect_dst(), 
                     p->get_angle(),
                     NULL, SDL_FLIP_NONE);
    }
      

    SDL_RenderPresent(this->rend);
  }
  
  if (this->quit)
    this->kill();
}

void 
Game::load_tex(std::shared_ptr<Player> player, 
               std::string path, 
               SDL_Rect * src, 
               SDL_Rect * dst)
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

inline SDL_Texture *
Player::get_tex(void)
{
  return this->texture;
}

inline SDL_Surface *
Player::get_surf(void)
{
  return this->player_surf;
}

inline void
Player::set_tex(SDL_Texture * tex)
{
  this->texture = tex;
}

inline void
Player::set_surf(SDL_Surface * surf)
{
  this->player_surf = surf;
}

Player::Player() {
  this->x = 640 / 2 - 8;
  this->y = 480 / 2 - 8;
}

inline void 
Player::set_x(double x) 
{
  this->x = x;
}

inline double
Player::get_x(void) 
{
  return this->x;
}

inline void 
Player::set_y(double y) 
{
  this->y = y;
}

inline double
Player::get_y(void) 
{
  return this->x;
}

inline void 
Player::set_angle(double a) 
{
  this->angle = a;
}

inline double
Player::get_angle(void) 
{
  return this->angle;
}

inline void 
Player::set_rect_src(SDL_Rect *rect) 
{
  this->src = rect;
}

inline void 
Player::set_rect_dst(SDL_Rect *rect) 
{
  this->dst = rect;
}

inline SDL_Rect *
Player::get_rect_dst(void) 
{
  return this->dst;
}

inline SDL_Rect *
Player::get_rect_src(void) 
{
  return this->src;
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
