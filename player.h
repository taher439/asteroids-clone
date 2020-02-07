#ifndef _PLAYER_H
#define _PLAYER_H
#include <memory>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <zlib.h>
#include <png.h>
#include <cmath>

class Player 
{
  private:
    double x, y, angle;
    std::shared_ptr<SDL_Texture> texture;
    std::shared_ptr<SDL_Surface> player_surf;
    std::shared_ptr<SDL_Rect>    dst, src;
    static constexpr double ship_thrust = 0.025;

  public:
    inline void           set_angle            (double a)
                            {this->angle = a;}

    inline void           set_x                (double x)
                            {this->x = x;}

    inline void           set_y                (double y)
                            {this->y = y;}

    inline void           set_surf             (std::shared_ptr<SDL_Surface> surf)
                            {this->player_surf = surf;}

    inline void           set_tex              (std::shared_ptr<SDL_Texture> tex)
                            {this->texture = tex;}

    inline void           set_rect_src         (std::shared_ptr<SDL_Rect> src)
                            {this->src = src;}

    inline void           set_rect_dst         (std::shared_ptr<SDL_Rect> dst)
                            {this->dst = dst;}

    inline std::shared_ptr<SDL_Surface>
                          get_surf             (void)
                            {return this->player_surf;}

    inline std::shared_ptr<SDL_Texture>
                          get_tex              (void)
                            {return this->texture;}

    inline std::shared_ptr<SDL_Rect>
                          get_rect_dst         (void)
                            {return this->dst;}

    inline std::shared_ptr<SDL_Rect>
                          get_rect_src         (void)
                            {return this->src;}

    inline double         get_angle            (void)
                            {return this->angle;}

    inline double         get_x                (void)
                            {return this->x;}

    inline double         get_y                (void)
                            {return this->y;}

                          Player               (void) {
                            this->x = 640 / 2 - 8;
                            this->y = 480 / 2 - 8;
                          }
    inline void           thrust               (double last_angle) {
                            this->x += Player::ship_thrust * cos(last_angle);
                            this->y -= Player::ship_thrust * sin(last_angle);

                            this->dst->x = this->x;
                            this->dst->y = this->y;
                          }
};

#endif