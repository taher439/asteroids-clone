#ifndef _PARTICLE_H
#define _PARTICLE_H
#include <algorithm>
#include <iostream>
#include <memory>
#include <SDL.h>
#include <list>
#include "templates.h"

// class Color
#define DFLT_MAX_TTL       15
#define DFLT_MIN_TTL       5

class Particle
{
  protected:
    // Color color;
    Vec2<double> position;
    Vec2<double> direction;
    double speed;

  public:
    Particle    (void);
    Particle    (Vec2<double>);
    Particle    (double x, double y): Particle(Vec2<double>(x, y)) {};

    virtual void step       (void);
    virtual void draw_particle  (const std::shared_ptr<SDL_Renderer>&);
    void set_position           (Vec2<double>);
    Vec2<double> get_position   (void);
    virtual bool is_active      (void);
};

class Ephemeral_particle: public Particle
{
  private:
    int time_to_live = 0;

  public:
    Ephemeral_particle   (void);
    Ephemeral_particle   (Vec2<double>, int, int);

    void step            (void) override;
    bool is_active       (void) override;
};

class Particle_container
{
  private:
    int total_particles = 0;
    std::list<std::shared_ptr<Particle>> particles;

  public:
    Particle_container();

    void add_particle     (std::shared_ptr<Particle> p);
    void step_all         (void);
    void draw_all         (const std::shared_ptr<SDL_Renderer>&);
    void clean_all        (void);
    bool is_empty         (void);
};

#endif