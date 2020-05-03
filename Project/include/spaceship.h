#ifndef _SPECESHIP_H_
#define _SPECESHIP_H_
#include "moving_object.h"

class Player;

class Spaceship: public Moving_object
{
  protected:
    std::vector<std::shared_ptr<Player>>  players;
    std::vector<std::shared_ptr<blast>>   blasts;
    int clock;
    int shoot_frequency;

  public:
    Spaceship         (const std::shared_ptr<SDL_Renderer>&, int, int, int);
    // init with spaceship position
    Spaceship         (const std::shared_ptr<SDL_Renderer>& r, Vec2<double> v, int h, int w, int f): Spaceship(r, h, w, f) { this->set_position(v.x,v.y);};
    Spaceship         (const std::shared_ptr<SDL_Renderer>& r, double x, double y, int h, int w, int f): Spaceship(r, h, w, f) { this->set_position(x,y);};

    bool              detect_inter            (const Vec2<double>&, const Vec2<double>&);
    // void              detect_player_collision (const std::vector<std::shared_ptr<Player>>&) override;
    void              detect_blast_collision  (std::vector<std::shared_ptr<blast>>&) override;
    void              update                  (std::vector<std::shared_ptr<blast>>&, const std::vector<std::shared_ptr<Player>>&) override;
    bool              has_type                (std::string);
                      
  private:
    void add_blast                (double);
    void draw_fire(void);
    void shoot_players            (const std::vector<std::shared_ptr<Player>>&);
    void create_texture           (int, int) override;

};
#endif 
