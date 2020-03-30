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
    Vec2<double> direction;
    double speed;

  public:
    bool detect_inter          (const Vec2<double>&, const Vec2<double>&);
    void draw_asteroid         (const std::shared_ptr<SDL_Renderer>&);
    void split_asteroid        (void);
    void detect_collision_ship (std::vector<std::shared_ptr<blast>>&);
    void move_asteroid         (void);
    void wrap_asteroid_coord   (Vec2<double>&);
         Asteroid              (int, double, double, double);
         Asteroid              (int x, Vec2<double> v, int y): Asteroid(x, v.x, v.y, y) {};

  private:
    Vec2<double> init_direction(void);


};
#endif 
