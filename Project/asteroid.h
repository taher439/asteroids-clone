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

    Vec2<int> center;
    std::vector<Vec2<int>> points;
    size current_size;

  public:
    void draw_asteroid         (void);
    void split_asteroid        (void);
    void detect_collision_ship (const std::shared_ptr<Player>&);
         Asteroid              (void);
};
#endif 
