#include "asteroid.h"
#include "player.h"
#include "sdl_wrapper.h"


Asteroid::Asteroid(const std::shared_ptr<SDL_Renderer>& rend, int vertices, int height, int width) :
Moving_object(rend, height, width)
{
  this->health = 100;
  double radius, angle, step, r, sp;
   
  radius = height/2;
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
  this->create_texture(height, width);
}

void 
Asteroid::detect_blast_collision(std::vector<std::shared_ptr<blast>>& blasts) 
{
  double dx, dy, dist; 
  
  for (auto s = blasts.begin(); s != blasts.end(); ++s) {
    dx = abs(this->center.x - (*s)->loc.x);
    dy = abs(this->center.y - (*s)->loc.y);
    dist = dx * dx + dy * dy;
    if (dist <= (this->height/2) * (this->height/2)) {
      #ifdef DEBUG
        std::cout << "\033[1;31mbold [*] collision detected\033[0m\n" << std::endl;
      #endif
      if (this->health > 0)
        this->health -= 25;
      blasts.erase(s);
      s--;
    }
  }
}

// kept for retro
void 
Asteroid::draw_asteroid() 
{
  auto prev = this->points[0] + this->center;
  auto first = prev;
  auto size = this->points.size();

  for (int i = 1; i < size; i++) {
    auto curr = this->points[i] + this->center;
    SDL_wrapper::draw_line( this->rend,
                            prev.x, prev.y,
                            curr.x, curr.y);
    prev = curr;
  }
  SDL_wrapper::draw_line( this->rend,
                          prev.x, prev.y,
                          first.x, first.y);
}

bool 
Asteroid::detect_inter(const Vec2<double>& A, const Vec2<double>& B)
{
  for (auto i: this->points)    
    if (Vec2<double>::dist(A, B, i+this->center))
      return true;
  return false;
}

void
Asteroid::update(const std::vector<std::shared_ptr<Player>>& players)
{
  this->move();
  for (auto p: players) {
    if (!p->is_alive()) {
      continue;
    }
    this->detect_blast_collision(p->get_blasts());
    if (!this->is_alive()) {
      // if asteroid died, update player score and leave function
      p->update_score(this->get_score());
      return;
    }
  }
  this->detect_player_collision(players);
}

void 
Asteroid::create_texture (int height, int width)
{
    this->texture = SDL_wrapper::create_texture(rend, height, width);
    SDL_SetTextureBlendMode(this->texture.get(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(this->rend.get(), this->texture.get());

    Vec2<double> texture_center = this->center - this->top_left;

    auto prev = this->points[0] + texture_center;
    auto first = prev;
    auto size = this->points.size();

    for (int i = 1; i < size; i++) {
        auto curr = this->points[i] + texture_center;
        SDL_wrapper::draw_line( this->rend,
                                prev.x, prev.y,
                                curr.x, curr.y);
        prev = curr;
    }
    SDL_wrapper::draw_line( this->rend,
                            prev.x, prev.y,
                            first.x, first.y);

    // draw blue texture box
    // SDL_Rect r = {0, 0, height, width};
    // SDL_SetRenderDrawColor(this->rend.get(), 0, 0, 255, 255);
    // SDL_RenderDrawRect(this->rend.get(), &r);
    // draw red hitbox
    // SDL_SetRenderDrawColor(this->rend.get(), 255, 0, 0, 255);
    // SDL_wrapper::draw_circle(this->rend, texture_center.x, texture_center.y, this->height/2);

    SDL_SetRenderTarget(this->rend.get(), nullptr);
}

bool       
Asteroid::has_type (std::string str)
{   
    if (str == "ASTEROID") {
        return true;
    } else {
        return false;
    }
}

int
Asteroid::get_score(void)
{
  int score;
  switch (this->height) {
    case (int)BIG:
      score = BIG_ASTEROID_SCORE;
      break;

    case (int)MID:
      score = MID_ASTEROID_SCORE;
      break;

    case (int)SMALL:
      score = SMALL_ASTEROID_SCORE;
      break;

    case (int)XSMALL:
      score = XSMALL_ASTEROID_SCORE;
      break;

    default:
      score = 0;
      break;
  }
  return score;
}