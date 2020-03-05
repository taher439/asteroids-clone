#ifndef _ASTEROID_H_
#define _ASTEROID_H_
#include "game.h"

class Asteroid
{
  private:
    enum size {
      BIG,
      MID,
      SMALL,
      XSMALL
    };

    std::vector<Vec2<double>> points;
    Vec2<double> center;
    size current_size;

  public:
    void draw_asteroid         (const std::shared_ptr<SDL_Renderer>&);
    void split_asteroid        (void);
    void detect_collision_ship (const std::shared_ptr<Player>&);
         Asteroid              (int, double&&, double&&);
};
#endif 
