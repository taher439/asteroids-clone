#include "asteroid.h"
#include "sdl_wrapper.h"

static inline double 
dist(const Vec2<double>& a, const Vec2<double>& b) 
{
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
};

static inline bool 
dist(const Vec2<double>& a, 
     const Vec2<double>& b, 
     const Vec2<double>& c) 
{
  return round(dist(a, c) + dist(c, b)) == round(dist(a, b));
}

Asteroid::Asteroid(int vertices, double x, double y, double size) 
{
  this->center.x = x;
  this->center.y = y;
  this->health = 100;
  this->direction = this->init_direction();
  this->speed = 1; // TODO: modify speed
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
    // this->points.emplace_back(tmp_v);
    this->points.emplace_back(tmp_v - this->center);
  }
}

Vec2<double>
Asteroid::init_direction()
{
  double angle = Rand_gen<double>::rand_num(0, 2) * PI;
  return Vec2<double> (cos(angle), sin(angle));
}

void 
Asteroid::draw_asteroid(const std::shared_ptr<SDL_Renderer>& rend) 
{
  // auto prev = this->points[0];
  // auto size = this->points.size();
  
  // for (int i = 1; i < size; i++) {
  //   SDL_wrapper::draw_line(rend, 
  //                          prev.x,
  //                          prev.y,
  //                          this->points[i].x,
  //                          this->points[i].y);
  //   prev = this->points[i];
  // }

  // SDL_wrapper::draw_line(rend, 
  //                        this->points[size - 1].x,
  //                        this->points[size - 1].y,
  //                        this->points[0].x,
  //                        this->points[0].y);
  auto prev = this->points[0] + this->center;
  auto first = prev;
  auto size = this->points.size();

  for (int i = 1; i < size; i++) {
    auto curr = this->points[i] + this->center;
    SDL_wrapper::draw_line( rend,
                            prev.x, prev.y,
                            curr.x, curr.y);
    prev = curr;
  }
  SDL_wrapper::draw_line( rend,
                          prev.x, prev.y,
                          first.x, first.y);
}

void 
Asteroid::detect_collision_ship(std::vector<std::shared_ptr<blast>>& blasts) 
{
  double dx, dy, dist; 
  std::list<int>  to_delete;
  
  for (int i = 0; i < blasts.size(); i++) {
    dx = abs(this->center.x - blasts[i]->loc.x);
    dy = abs(this->center.y - blasts[i]->loc.y);
    dist = dx * dx + dy * dy;
    if (dist <= this->current_size * this->current_size) {
      #ifdef DEBUG
        std::cout << "\033[1;31mbold [*] collision detected\033[0m\n" << std::endl;
      #endif
      if (this->health > 0)
        this->health -= 25;
      to_delete.emplace_back(i);
    }
  }

  for (auto i: to_delete) {
    blasts.erase(blasts.begin() + i);
  }
}

bool 
Asteroid::detect_inter(const Vec2<double>& A, const Vec2<double>& B)
{
  for (auto i: this->points)    
    if (dist(A, B, i+this->center))
    return true;
  return false;
}

void 
Asteroid::wrap_asteroid_coord (Vec2<double>& v) 
{ // TODO: use globals for screen size
  if (v.x > 640) 
    v.x = 0;
  if (v.x < 0) 
    v.x = 640;
  
  if (v.y > 480) 
    v.y = 0;
  if (v.y < 0) 
    v.y = 480;
}

void
Asteroid::move_asteroid(void)
{
  this->center += this->direction;
  this->wrap_asteroid_coord(this->center);
}