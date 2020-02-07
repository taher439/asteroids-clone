#include "player.h"
#include "game.h"

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