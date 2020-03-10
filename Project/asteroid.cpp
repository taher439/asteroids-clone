#include "asteroid.h"
#include "sdl_wrapper.h"

Asteroid::Asteroid(int vertices, double&& x, double&& y, double size) 
{
  this->center.x = std::move(x);
  this->center.y = std::move(y);
  double radius, angle, step, r, sp;
   
  radius = size;
  sp = 0.5 * radius; 

  step = 1.0 / vertices;
  angle = 0;
  Vec2<double> tmp_v;
  while (angle < 2.0 * PI) {
    r = Rand_gen<double>::rand_num(radius, sp);
    tmp_v.x  = cos(angle) * r + this->center.x;
    tmp_v.y  = sin(angle) * r + this->center.y;
    angle += (step * 
              (2.0 * PI) / vertices + 
              (40.0 * Rand_gen<double>::rand_num(0, 1))) 
              * PI / 180.0;
    
#ifdef DEBUG
    std::cout << "[!] polygon x: " 
              << tmp_v.x << " " 
              << "polygon y: " 
              << tmp_v.y << std::endl;
#endif
    this->points.emplace_back(tmp_v);
  }
}

void Asteroid::draw_asteroid(const std::shared_ptr<SDL_Renderer>& rend) 
{
  auto prev = this->points[0];
  auto size = this->points.size();
  
  for (int i = 1; i < size; i++) {
    SDL_wrapper::draw_line(rend, 
                           prev.x,
                           prev.y,
                           this->points[i].x,
                           this->points[i].y);
    prev = this->points[i];
  }

  SDL_wrapper::draw_line(rend, 
                         this->points[size - 1].x,
                         this->points[size - 1].y,
                         this->points[0].x,
                         this->points[0].y);
}
