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
#include "game.h"

class Player 
{
  private:
    double x, y, angle = 90 / 180 * PI;
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    std::shared_ptr<SDL_Texture> texture;
    std::shared_ptr<SDL_Surface> player_surf;
    std::shared_ptr<SDL_Rect>    dst, src;
    static constexpr double ship_thrust = 5;
    double thrust_x, thrust_y;

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
                            this->thrust_x += Player::ship_thrust * sin(last_angle) / 60;
                            this->thrust_y -= Player::ship_thrust * cos(last_angle) / 60;
                          }

    inline void           move_ship            (void) {
                            this->x += this->thrust_x;
                            this->y += this->thrust_y;
                            this->dst->x = this->x;
                            this->dst->y = this->y;
    }

    inline void           slow_ship            (void) {
                            this->thrust_x -= 0.005 * this->thrust_x;
                            this->thrust_y -= 0.005 * this->thrust_y;
    }

    inline void           wrap_ship            (void) {
                            if (this->x > this->SCREEN_WIDTH) 
                              this->x = 0;
                            if (this->x < 0) 
                              this->x = this->SCREEN_WIDTH;
                            
                            if (this->y > this->SCREEN_HEIGHT) 
                              this->y = 0;
                            if (this->y < 0) 
                              this->y = this->SCREEN_HEIGHT;
    }

    void                 draw_ship            (std::shared_ptr<SDL_Renderer>);
};

#endif