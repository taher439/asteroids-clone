#include "asteroid.h"
#include "sdl_wrapper.h"

Asteroid::Asteroid(int vertices, double x, double y, double size) 
{
  this->center.x = x;
  this->center.y = y;
  double radius, angle, step, r, sp;
   
  radius = size;
  this->current_size = size;
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

void 
Asteroid::draw_asteroid(const std::shared_ptr<SDL_Renderer>& rend) 
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

void 
Asteroid::detect_collision_ship(std::vector<std::shared_ptr<blast>>& blasts) 
{
  double dx, dy, dist; 
  for (auto s = blasts.begin(); s != blasts.end(); ++s) {
    dx = abs(this->center.x - (*s)->loc.x);
    dy = abs(this->center.y - (*s)->loc.y);
    dist = dx * dx + dy * dy;
    if (dist <= this->current_size * this->current_size) {
      #ifdef DEBUG
        std::cout << "\033[1;31mbold [*] collision detected\033[0m\n" << std::endl;
      #endif
      blasts.erase(s);
      s--;
    }
  }
}

void 
Asteroid::detect_inter(const Vec2<double>& A, const Vec2<double>& B)
{
  for (auto i: this->points) {

  }
}