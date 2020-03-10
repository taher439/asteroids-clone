#include "asteroid.h"
#include "sdl_wrapper.h"

Asteroid::Asteroid(int vertices, double&& x, double&& y) 
{
  this->center.x = std::move(x);
  this->center.y = std::move(y);

  double radius, angle;
  Vec2<double> tmp;
  radius = Rand_gen<double>::rand_num(0.0, 10.0); 
  while (vertices > 0) {
    angle  = Rand_gen<double>::rand_num(0.0, 1.0) * PI * 2;
    tmp.x  = cos(angle) * radius + this->center.x;
    tmp.y  = sin(angle) * radius + this->center.y;

#ifdef DEBUG
    std::cout << "[!] polygon x: " 
              << tmp.x << " " 
              << "polygon y: " 
              << tmp.y << std::endl;
#endif 

    this->points.emplace_back(tmp);
    vertices--;
  }
}

void Asteroid::draw_asteroid(const std::shared_ptr<SDL_Renderer>& rend) 
{
  auto prev = this->points[0];
  auto size = this->points.size();
    
  for (int i = 1; i < size; i++) 
    SDL_wrapper::draw_line(rend, 
                           prev.x,
                           prev.y,
                           this->points[i].x,
                           this->points[i].y);

  SDL_wrapper::draw_line(rend, 
                         this->points[size - 1].x,
                         this->points[size - 1].y,
                         this->points[0].x,
                         this->points[0].y);
}
