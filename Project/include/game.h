#ifndef _GAME_H
#define _GAME_H
#include <stdlib.h>
#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <zlib.h>
#include <png.h>
#include <memory>
#include <type_traits>
#include <cmath>
#include <thread>
#include <mutex>
#include <list>
#include <map>
#include <templates.h>

#define PI 3.14159265
#define MAX_PARTICLES 20
#ifdef AZERTY
  #define LEFT      SDLK_q
  #define RIGHT     SDLK_d
  #define LEFT_ALT  SDLK_k
  #define RIGHT_ALT SDLK_m
#else
  #define LEFT      SDLK_a
  #define RIGHT     SDLK_d
  #define LEFT_ALT  SDLK_k
  #define RIGHT_ALT SDLK_m
#endif
class Player;
class Asteroid;
class Particle_container;
class Moving_object;

typedef struct sdl_event_handler
{
  std::shared_ptr<SDL_Event> ev;
  double sprite_angle;
  std::mutex mu;
  bool   thrust,
         quit, 
         blast, 
         second_p;
  int    FPS;
} sdl_event_handler;

class Direction
{
  public:
    static Vec2<double> normed () {
      double angle = Rand_gen<double>::rand_num(0, 2) * PI;
      return Vec2<double> (cos(angle), sin(angle));
    }
};

class Game 
{
  private:
    bool mp;
    int total_asteroids;
    std::shared_ptr<SDL_Window>   win;
    std::shared_ptr<SDL_Surface>  surf;
    std::shared_ptr<SDL_Renderer> rend;
    std::shared_ptr<SDL_Event>    ev;
    std::map<char, std::shared_ptr<SDL_Texture>> numbers;
    std::list<std::shared_ptr<Particle_container>> particle_clouds;
    std::list<std::shared_ptr<Moving_object>> moving_objects;
    std::vector<std::shared_ptr<Player>>   players;
    int current_level;

    enum state {
      MENU,
      GAME,
      CONFIG
    };

    state current_state;
    
  public:
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;

    void                  load_tex             (const std::shared_ptr<Player>&,
                                                const std::string&, 
                                                std::shared_ptr<SDL_Rect>&&, 
                                                std::shared_ptr<SDL_Rect>&&);

    void                  kill                 (void);
    void                  init                 (int, bool);
    inline void           set_win              (std::shared_ptr<SDL_Window>&&);
    inline void           set_surf             (std::shared_ptr<SDL_Surface>&&);
    inline void           set_rend             (std::shared_ptr<SDL_Renderer>&&);

    inline std::shared_ptr
    <SDL_Renderer> 
                          get_rend             (void)
                            {return this->rend;}

    inline std::shared_ptr
    <SDL_Surface>
                          get_surf             (void)
                            {return this->surf;}

    inline std::shared_ptr
    <SDL_Window>
                          get_win              (void)
                            {return this->win;}

                          Game                 (const int& width, const int& height): 
                                                  SCREEN_WIDTH(width), SCREEN_HEIGHT(height) {}
    
    void                  proc_input            (void);
    int                   split_asteroid        (Vec2<double>, double);
    std::shared_ptr
    <Particle_container>  generate_particles    (Vec2<double>, int);

    // game update functions
    void                  display_ui            (void);
    void                  particles             (void);
    void                  update_objects        (void);
    void                  update_players        (void);
    void                  level_up              (void);
    void                  spawn_spaceship       (int);
    void                  spawn_asteroid        (int, int, int);

    void                  hitboxes_test         (void);
};

#endif 
