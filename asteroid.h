#ifndef _ASTEROID_H_
#define _ASTEROID_H_
#include "game.h"

class Asteroid
{
  private:
    enum size {
      large,
      medium,
      small,
      tiny
    };

    double x, y;
    std::vector<point> points;
    size current_size;

  public:
    void draw_asteroid         (void);
    void split_asteroid        (void);
    void detect_collision_ship (std::shared_ptr<Player>);
         Asteroid              (void);
};
#endif 
