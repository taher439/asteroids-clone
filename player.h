#ifndef _PLAYER_H
#define _PLAYER_H
#include "game.h"

class Player 
{
  private:
    point center;
    double angle = 90 / 180 * PI;
    const int SCREEN_WIDTH  = 640;
    const int SCREEN_HEIGHT = 480;
    std::shared_ptr<SDL_Texture> texture;
    std::shared_ptr<SDL_Surface> player_surf;
    std::shared_ptr<SDL_Rect>    dst, src;
    static constexpr double      ship_thrust = 5;
    double thrust_x, thrust_y;
    int lives = 3, score = 0;

  public:
    inline void           set_angle            (double a)
                            {this->angle = std::move(a);}

    inline point         get_center            (void)
                            {return this->center;}
    inline void          set_center            (point c)
                            {this->center = std::move(c);}
    inline void           set_surf             (std::shared_ptr<SDL_Surface> s)
                            {this->player_surf = std::move(s);}

    inline void           set_tex              (std::shared_ptr<SDL_Texture> t)
                            {this->texture = std::move(t);}

    inline void           set_rect_src         (std::shared_ptr<SDL_Rect> src)
                            {this->src = std::move(src);}

    inline void           set_rect_dst         (std::shared_ptr<SDL_Rect> dst)
                            {this->dst = std::move(dst);}

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

                          Player               (void) 
                          {
                            this->center.x = 640 / 2 - 8;
                            this->center.y = 480 / 2 - 8;
                          }
                          
    inline void           thrust               (const double& last_angle) {
                            this->thrust_x += 
                              Player::ship_thrust * sin(last_angle) / 60;
                            this->thrust_y -= 
                              Player::ship_thrust * cos(last_angle) / 60;
                          }

    inline void           move_ship            (void) 
                          {
                            this->center.x += this->thrust_x;
                            this->center.y += this->thrust_y;
                          }

    inline void           slow_ship            (void) 
                          {
                            this->thrust_x -= 0.005 * this->thrust_x;
                            this->thrust_y -= 0.005 * this->thrust_y;
                          }

    inline void           wrap_ship            (void) 
                          {
                            if (this->center.x > this->SCREEN_WIDTH) 
                              this->center.x = 0;
                            if (this->center.x < 0) 
                              this->center.x = this->SCREEN_WIDTH;
                            
                            if (this->center.y > this->SCREEN_HEIGHT) 
                              this->center.y = 0;
                            if (this->center.y < 0) 
                              this->center.y = this->SCREEN_HEIGHT;
                          }

    void                 draw_ship            (std::shared_ptr<SDL_Renderer>,
                                               const bool&);
};

#endif
