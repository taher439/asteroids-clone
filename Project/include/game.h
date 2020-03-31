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
#include <random>
#include <mutex>
#include <list>

#define PI 3.14159265
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

    inline Vec2<T> operator-(const Vec2& v) const {
      return Vec2(this->x - v.x, this->y - v.y);
    }

    inline Vec2<T> operator*(const T& val) const {
      return Vec2(this->x * val, this->y * val);
    }

    inline Vec2<T> operator+(const T& val) const {
      return Vec2(this->x + val, this->y + val);
    }

    inline Vec2<T> operator-(const T& val) const {
      return Vec2(this->x - val, this->y - val);
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

    inline friend std::ostream& operator << (std::ostream &out, const Vec2<T>& v){
      out << v.x << " | " << v.y;
      return out;
    }
  
    void print() {
#ifdef DEBUG
      std::cout << this->x << " " << this->y << std::endl;
#endif
    }
};

typedef struct blast
{
  Vec2<double> loc;
  double angle;
} blast;


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
    bool mp;
    int total_asteroids;
    std::shared_ptr<SDL_Window>   win;
    std::shared_ptr<SDL_Surface>  surf;
    std::shared_ptr<SDL_Renderer> rend;
    std::shared_ptr<SDL_Event>    ev;
    std::vector<std::shared_ptr<Asteroid>> active_asteroids;
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
    
    void                 proc_input            (void);
    int                  split_asteroid        (Vec2<double>, double);
};
#endif 
