#ifndef _MOVING_OBJECT_H_
#define _MOVING_OBJECT_H_
#include <stdlib.h>
#include <SDL.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <list>
#include "templates.h"
#define BIG 100.0
#define MID 50.0
#define SMALL 25.0
#define XSMALL 12.5
#define DEFAULT_SPACESHIP_HEIGHT 20.0
#define DEFAULT_SPACESHIP_WIDTH 41.0

#define BIG_ASTEROID_SCORE 50
#define MID_ASTEROID_SCORE 70
#define SMALL_ASTEROID_SCORE 100
#define XSMALL_ASTEROID_SCORE 150
#define SPACESHIP_SCORE 0

class Player;

class Moving_object
{
  protected:
    std::shared_ptr<SDL_Renderer> rend;     // renderer on which the texture is drawn
    std::shared_ptr<SDL_Texture>  texture;
    SDL_Rect                      dstrect;  // rectangle used to draw the texture
    Vec2<double>                  direction;
    Vec2<double>                  top_left;
    Vec2<double>                  center;
    double                        speed;
    int                           height, width;
    double                        health;

  public:
    Moving_object        (void);
    Moving_object        (const std::shared_ptr<SDL_Renderer>&, int, int);

    void draw           ();
    void set_position   (double, double);
    void set_size       (int, int);
    void set_speed      (double);
    void set_health     (int);

    inline int            get_height        (void) { return this->height; };
    inline int            get_width         (void) { return this->width; };
    inline double         get_health        (void) { return this->health;};
    inline bool           is_alive          (void) { return this->health > 0;};
    inline const Vec2<double>&  get_center  (void) { return this->center; };

    // game update functions
    void detect_player_collision(const std::vector<std::shared_ptr<Player>>&);

    // virtual game update functions
    virtual void update (const std::vector<std::shared_ptr<Player>>&) = 0;
    virtual void detect_blast_collision(std::vector<std::shared_ptr<blast>>&) = 0; 
    virtual bool has_type (std::string) = 0;
    virtual int  get_score (void) = 0; 

  protected:
    void move         (void);
    void wrap_coord   (Vec2<double>& v);
    bool in_screen    (const Vec2<double>&);
    Vec2<double>      generate_center_pos (void);
    virtual bool      detect_inter (const Vec2<double>&, const Vec2<double>&) = 0;
    virtual void      create_texture   (int, int) = 0;
};

// class Moving_object_container
// {
//   private:
//     int total_objects = 0;
//     std::list<std::shared_ptr<Moving_object>> moving_objects;

//   public:
//     Moving_object_container();

//     void add_moving_object      (std::shared_ptr<Moving_object> p);
//     void update_all             (std::vector<std::shared_ptr<blast>>&);
//     void draw_all               (void);
//     void clean_all              (void);
//     bool is_empty               (void);
// };

#endif 