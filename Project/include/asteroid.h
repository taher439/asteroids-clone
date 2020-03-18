#ifndef _ASTEROID_H_
#define _ASTEROID_H_
#include "game.h"
#define BIG 50.0
#define MID 25.0
#define SMALL 12.5
#define XSMALL 6.25

class Asteroid
{
  private:
    std::vector<Vec2<double>> points;
    Vec2<double> center;
    double current_size;

  public:
    bool detect_inter          (const Vec2<double>&, const Vec2<double>&);
    void draw_asteroid         (const std::shared_ptr<SDL_Renderer>&);
    void split_asteroid        (void);
    void detect_collision_ship (std::vector<std::shared_ptr<blast>>&);
         Asteroid              (int, double, double, double);
         Asteroid              (int x, Vec2<double> v, int y): Asteroid(x, v.x, v.y, y) {};
};
#endif 
