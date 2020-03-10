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
    void draw_asteroid         (const std::shared_ptr<SDL_Renderer>&);
    void split_asteroid        (void);
    void detect_collision_ship (const std::shared_ptr<Player>&);
         Asteroid              (int, double&&, double&&, double);
};
#endif 
