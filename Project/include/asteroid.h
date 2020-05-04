#ifndef _ASTEROID_H_
#define _ASTEROID_H_
#include "moving_object.h"

class Asteroid: public Moving_object
{
  protected:
    std::vector<Vec2<double>> points;

  public:
    // init without asteroid position
    Asteroid          (const std::shared_ptr<SDL_Renderer>&, int, int, int);
    // init with asteroid position
    Asteroid          (const std::shared_ptr<SDL_Renderer>& r, int x, Vec2<double> v, int h, int w): Asteroid(r, x, h, w) { this->set_position(v.x,v.y);};
    Asteroid          (const std::shared_ptr<SDL_Renderer>& r, int v, double x, double y, int h, int w): Asteroid(r, v, h, w) { this->set_position(x,y);};
    
    bool              detect_inter            (const Vec2<double>&, const Vec2<double>&);
    void              draw_asteroid           (); // retrocompatibility (no textures)
    // void              detect_player_collision (const std::vector<std::shared_ptr<Player>>&) override;
    void              detect_blast_collision  (std::vector<std::shared_ptr<blast>>&) override;
    void              update                  (const std::vector<std::shared_ptr<Player>>&) override;
    bool              has_type                (std::string) override;
    int               get_score               (void) override;

  private:
    void draw_from_center         (const Vec2<double>&);
    void create_texture           (int, int) override;

};
#endif 
