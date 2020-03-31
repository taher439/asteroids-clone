#ifndef _PLAYER_H
#define _PLAYER_H
#include "game.h"

class Player 
{
  private:
    Vec2<double> center;
    double angle = 90 / 180 * PI;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    std::shared_ptr<SDL_Texture> texture;
    std::shared_ptr<SDL_Surface> player_surf;
    std::shared_ptr<SDL_Rect>    dst, src;
    static constexpr double      ship_thrust = 5;
    Vec2<double> thrust_vec, A, B, C, D, E, F;
    int lives = 3, score = 0;

  public:
    std::vector<std::shared_ptr<blast>>  blasts;
    sdl_event_handler hdl;
    bool second_p;

    inline void           set_angle            (double a)
                            {this->angle = a;}

    inline Vec2<double>   get_center            (void)
                            {return this->center;}

    inline void          set_center            (Vec2<double>&& c)
                            {this->center = std::move(c);}

    inline void           set_surf             (std::shared_ptr<SDL_Surface>&& s)
                            {this->player_surf = std::move(s);}

    inline void           set_tex              (std::shared_ptr<SDL_Texture>&& t)
                            {this->texture = std::move(t);}

    inline void           set_rect_src         (std::shared_ptr<SDL_Rect>&& src)
                            {this->src = std::move(src);}

    inline void           set_rect_dst         (std::shared_ptr<SDL_Rect>&& dst)
                            {this->dst = std::move(dst);}

    inline std::shared_ptr
    <SDL_Surface>
                          get_surf             (void)
                            {return this->player_surf;}

    inline std::shared_ptr
    <SDL_Texture>
                          get_tex              (void)
                            {return this->texture;}

    inline std::shared_ptr
    <SDL_Rect>
                          get_rect_dst         (void)
                            {return this->dst;}

    inline std::shared_ptr
    <SDL_Rect>
                          get_rect_src         (void)
                            {return this->src;}

    inline double         get_angle            (void)
                            {return this->angle;}

                          Player               (int width, int height) 
                          {
                            this->SCREEN_HEIGHT = height;
                            this->SCREEN_WIDTH = width;
                            this->center     = Vec2<double>(width / 2 - 8, height / 2 - 8);
                            this->thrust_vec = Vec2<double>();
                            this->hdl.sprite_angle = 0;
                            this->hdl.thrust = false;
                            this->hdl.FPS = 60;
                            this->hdl.quit = false;
                            this->hdl.blast = false;
                            this->hdl.second_p = false;
                          }
                          
    inline void           thrust               (const double& last_angle) {
                          this->thrust_vec.x += 
                              Player::ship_thrust * sin(last_angle) / 60;
                            this->thrust_vec.y -= 
                              Player::ship_thrust * cos(last_angle) / 60;
                          
                            this->thrust_vec.print();
                          }

    inline void           move_ship            (void) 
                          {
                            this->center += this->thrust_vec;
                          }

    inline void           slow_ship            (void) 
                          {
                            this->thrust_vec.x -= 0.005 * this->thrust_vec.x;
                            this->thrust_vec.y -= 0.005 * this->thrust_vec.y;
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
    void                 add_blast            (const double&);
    void                 draw_fire            (const std::shared_ptr<SDL_Renderer>&);
    void                 asteroid_collision   (const std::shared_ptr<Asteroid>&);
    void                 draw_ship            (const std::shared_ptr<SDL_Renderer>&,
                                               const bool&);
};

#endif