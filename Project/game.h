#ifndef _GAME_H
#define _GAME_H
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <zlib.h>
#include <png.h>
#include <memory>
#include <type_traits>
#include <cmath>
#include <random>

#define PI 3.14159265
#ifdef AZERTY
  #define LEFT SDLK_q
  #define RIGHT SDLK_d
#else
  #define LEFT SDLK_a
  #define RIGHT SDLK_d
#endif
class Player;
class Asteroid;

template <typename T>
class Vec2 {
  static_assert(std::is_arithmetic_v<T>);
  public:
    T x, y;
    Vec2(): x(0), y(0) {}
    Vec2(const T& _x, const T& _y): x(_x), y(_y) {}
    Vec2(const Vec2<T>& v): x(v.x), y(v.y) {}
    inline Vec2<T> operator+(const Vec2& v) const {
      return Vec2(this->x + v.x, this->y + v.y);
    }

    inline Vec2<T> operator*(const T& val) const {
      return Vec2(this->x * val, this->y * val);
    }

    inline Vec2<T>& operator += (const Vec2<T>& v) {
      x += v.x;
      y += v.y;
      return *this;
    }

    inline Vec2<T>& operator += (const T& val) {
      x += val;
      y += val;
      return *this;
    }

    inline Vec2<T>& operator -= (const Vec2<T>& v){
      x -= v.x;
      y -= v.y;
      return *this;
    }

    inline Vec2<T>& operator *= (const T& s){
    x *= s;
    y *= s;
    return *this;
    }
  
    inline Vec2<T>& operator *= (const Vec2<T>& v){
      x *= v.x;
      y *= v.y;
      return *this;
    }

    inline Vec2<T>& operator /= (const T& s){
      x /= s;
      y /= s;
      return *this;
    }
  
    void print() {
#ifdef DEBUG
      std::cout << this->x << " " << this->y << std::endl;
#endif
    }
};

template <typename T, typename = void>
class Rand_gen;

template <typename T>
class Rand_gen <T, std::enable_if_t<std::is_floating_point_v<T>>>
{
  public:
    static T rand_num(const T& min, const T& max) {
      std::random_device rd; 
      std::mt19937 gen(rd()); 
      std::uniform_real_distribution<T> dis(min, max);
      return dis(gen);
    }

    static T rand_gauss(const T& min, const T& max) {
      std::random_device rd; 
      std::mt19937 gen(rd()); 
      std::normal_distribution<T> dis(min, max);
      return dis(gen);
    }
};

template <typename T>
class Rand_gen <T, std::enable_if_t<std::is_integral_v<T>>>
{
  public:
    static T rand_num(const T& min, const T& max) {
      std::random_device rd; 
      std::mt19937 gen(rd()); 
      std::uniform_int_distribution<T> dis(min, max);
      return dis(gen);
    }
  
    static T rand_gauss(const T& min, const T& max) {
      std::random_device rd; 
      std::mt19937 gen(rd()); 
      std::normal_distribution<T> dis(min, max);
      return dis(gen);
    }
};

class Game 
{
  private:
    int total_asteroids;
    std::shared_ptr<SDL_Window>   win;
    std::shared_ptr<SDL_Surface>  surf;
    std::shared_ptr<SDL_Renderer> rend;

    SDL_Event ev;
    std::vector<std::shared_ptr<Asteroid>> active_asteroids;
    std::vector<std::shared_ptr<Player>>   players;
    std::shared_ptr<Player>                main_player;
    

    enum state {
      MENU,
      GAME,
      CONFIG
    };

    state current_state;
    
  public:
    const int SCREEN_WIDTH  = 640;
    const int SCREEN_HEIGHT = 480;
    bool quit = false;

    void                  load_tex             (const std::shared_ptr<Player>&,
                                                const std::string&, 
                                                std::shared_ptr<SDL_Rect>&&, 
                                                std::shared_ptr<SDL_Rect>&&);

    void                  kill                 (void);
    void                  init                 (int&&);
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
    
    inline std::shared_ptr
    <Player> 
                         get_main_player       (void)
                           {return this->main_player;}

    inline void          set_main_player       (std::shared_ptr<Player>&& p)
                           {
                             this->main_player = std::move(p);
                             this->players.push_back(this->main_player);
                           }
                           
    void                 proc_input            (void);
};
#endif 
