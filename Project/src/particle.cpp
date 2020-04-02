#include "particle.h"
#include "sdl_wrapper.h"

////////////////////////////////////////////////////////////////////////////////
Particle::Particle ()
{
  this->direction = Direction::normed();
  this->speed = Rand_gen<double>::rand_num(1.5, 4.0);
}

Particle::Particle (Vec2<double> v)
{
  this->position = v;
  this->direction = Direction::normed();
  this->speed = Rand_gen<double>::rand_num(0.5, 3.0);
  // this->speed = 1;
}

void 
Particle::step (void)
{
  this->position += this->direction;
}

void 
Particle::draw_particle (const std::shared_ptr<SDL_Renderer>& rend)
{
  SDL_wrapper::draw_point( rend, 
                           this->position.x, 
                           this->position.y);
}

void 
Particle::set_position (Vec2<double> pos)
{
  this->position = pos;
}

Vec2<double> 
Particle::get_position (void)
{
  return this->position;
}

bool
Particle::is_active (void)
{
  return true;
}

////////////////////////////////////////////////////////////////////////////////
Ephemeral_particle::Ephemeral_particle():
Particle()
{
  this->time_to_live = Rand_gen<int>::rand_num(DFLT_MIN_TTL, DFLT_MAX_TTL);
}

Ephemeral_particle::Ephemeral_particle(Vec2<double> pos, int min, int max):
Particle(pos)
{
  this->time_to_live = Rand_gen<int>::rand_num(min, max);
}

void 
Ephemeral_particle::step (void)
{
  Particle::step();
  this->time_to_live--;
}

bool
Ephemeral_particle::is_active (void)
{
  return this->time_to_live != 0;
}

////////////////////////////////////////////////////////////////////////////////
Particle_container::Particle_container ()
{
  this->total_particles = 0;
}

void
Particle_container::add_particle(std::shared_ptr<Particle> p)
{
  this->total_particles++;
  this->particles.emplace_back(p);
}

void 
Particle_container::step_all (void)
{
  for (auto p = this->particles.begin(); p != this->particles.end(); p++) {
    (*p)->step();
  }
}

void 
Particle_container::draw_all (const std::shared_ptr<SDL_Renderer>& rend)
{
  for (auto p = this->particles.begin(); p != this->particles.end(); p++) {
    (*p)->draw_particle(rend);
  }
}

void 
Particle_container::clean_all (void)
{
  for (auto p = this->particles.begin(); p != this->particles.end(); p++) {
    if ( !(*p)->is_active() ) {
      p = this->particles.erase(p);
      p--;
      this->total_particles--;
    }
  }
}

bool
Particle_container::is_empty (void)
{
  return this->total_particles == 0;
}