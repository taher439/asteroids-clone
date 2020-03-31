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
  this->fake_center.x = x;
  this->fake_center.y = y;
  this->has_fake_center = false;
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
Asteroid::draw_from_center(const std::shared_ptr<SDL_Renderer>& rend, const Vec2<double>& center)
{
  auto prev = this->points[0] + center;
  auto first = prev;
  auto size = this->points.size();

  for (int i = 1; i < size; i++) {
    auto curr = this->points[i] + center;
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
Asteroid::draw_asteroid(const std::shared_ptr<SDL_Renderer>& rend) 
{
  bool at_least_one_out_flag = false;
  bool all_out_flag = true;
  Vec2<int> side(0,0); // used to know on which side we went out of the screen
  for (auto p : this->points) {
    if (this->point_in_screen(p + this->center, side) ) {
      // at least one point was still in screen
      all_out_flag = false;
    } else {
      at_least_one_out_flag = true;
    }
  }

  if (at_least_one_out_flag) {
    // if at least one point is out of the screen
    if (!this->has_fake_center) {
      this->has_fake_center = true;
      this->fake_center.x -= 640 * side.x;
      this->fake_center.y -= 480 * side.y;
    }
  } 
  if (all_out_flag) {
    // // all the points are back in the screen from the other side
    this->has_fake_center = false;
    // reset fake center position
    this->center = this->fake_center;
  }

  this->draw_from_center(rend, this->center);
  if (this->has_fake_center) {
    this->draw_from_center(rend, this->fake_center);
  }
}

void 
Asteroid::detect_collision_ship(std::vector<std::shared_ptr<blast>>& blasts)
{
  double dist_center, dist_fake_center; 
  std::list<int>  to_delete;
  Vec2<double> d1, d2;
  for (int i = 0; i < blasts.size(); i++) {
    d1 = Vec2<double>(abs(this->center.x - blasts[i]->loc.x), 
                      abs(this->center.y - blasts[i]->loc.y));
    d2 = Vec2<double>(abs(this->fake_center.x - blasts[i]->loc.x), 
                      abs(this->fake_center.y - blasts[i]->loc.y));
    dist_center = d1.x * d1.x + d1.y * d1.y;
    dist_fake_center = d2.x * d2.x + d2.y * d2.y;
    if (dist_center <= this->current_size * this->current_size
        || dist_fake_center <= this->current_size * this->current_size ) {
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

bool 
Asteroid::point_in_screen (const Vec2<double>& v, Vec2<int>& side)
{ // TODO: use globals for screen size
  // side vector represent the screen side overshot
  bool in_flag = true;
  if (v.x > 640) {
    in_flag = false;
    side.x = 1;
  }
  if (v.x < 0) {
    in_flag = false;
    side.x = -1;
  }
  
  if (v.y > 480) {
    in_flag = false;
    side.y = 1;
  }
  if (v.y < 0) {
    in_flag = false;
    side.y = -1;
  }
  return in_flag;
}

void
Asteroid::move_asteroid(void)
{
  this->center += this->direction * this->speed;
  this->fake_center += this->direction * this->speed;
}