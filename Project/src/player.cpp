#include "player.h"
#include "asteroid.h"
#include "sdl_wrapper.h"

void
Player::calculate_pos(void)
{
  //first segment
  this->A = Vec2<double>(this->center.x + 10 * sin(this->angle), 
              this->center.y - 10 * cos(this->angle));

  this->B = Vec2<double>(this->center.x - 10 * (sin(this->angle) + cos(this->angle)),
              this->center.y + 10 * (cos(this->angle) - sin(this->angle)));

  //second segment
  this->C = Vec2<double>(this->center.x + 10 * sin(this->angle),
              this->center.y - 10 * cos(this->angle));

  this->D = Vec2<double>(this->center.x - 10 * (sin(this->angle) - cos(this->angle)),
              this-> center.y + 10 * (cos(this->angle) + sin(this->angle)));

  //third segment
  this->E = Vec2<double>(this->center.x - 7 * (sin(this->angle) + cos(this->angle)),
              this->center.y + 7 * (cos(this->angle) - sin(this->angle)));
                  
  this->F = Vec2<double>(this->center.x - 7 * (sin(this->angle) - cos(this->angle)),
              this->center.y + 7 * (cos(this->angle) + sin(this->angle)));
}

void 
Player::draw_ship(const std::shared_ptr<SDL_Renderer>& rend, const bool& thrusting)
{ 
  this->calculate_pos();

  SDL_wrapper::draw_line(rend, A.x, A.y, B.x, B.y);
  SDL_wrapper::draw_line(rend, C.x, C.y, D.x, D.y);
  SDL_wrapper::draw_line(rend, E.x, E.y, F.x, F.y);

  if (thrusting) {
      //add thrust animation
  }
}

void 
Player::add_blast()
{
  std::shared_ptr<blast> tmp = std::make_shared<blast>();
  tmp->loc = Vec2<double>(this->center);
  tmp->angle = this->angle;
  #ifdef DEBUG
    std::cout << "[*]vector blast tmp ";
    tmp->loc.print();
  #endif
  this->blasts.push_back(tmp);
}

void 
Player::draw_fire(const std::shared_ptr<SDL_Renderer>& rend)
{
    if (this->blasts.empty()) {
        return;
    }

    for (auto s: this->blasts) {
        s->loc.x += cos(s->angle-PI/2) * 5;
        s->loc.y += sin(s->angle-PI/2) * 5;
        SDL_wrapper::draw_point(rend, (int) s->loc.x, (int) s->loc.y);
        #ifdef DEBUG
            std::cout << "current angle " << angle << std::endl; 
        #endif
    }

    for (auto s = this->blasts.begin(); s != this->blasts.end(); ++s) {
         #ifdef DEBUG
            std::cout << "[!] blast vector size " 
                      << this->blasts.size() << std::endl; 
        #endif

        if ((*s)->loc.x > this->SCREEN_WIDTH 
            || ((*s)->loc.x < 0) 
            || ((*s)->loc.y > this->SCREEN_HEIGHT) 
            || ((*s)->loc.y < 0)) {
            this->blasts.erase(s);
            s--;
        }
    }
}

// function called when there was a collision
void 
Player::take_damage(void)
{
  if (this->is_invincible())
    return;

  Vec2<double> new_center (SCREEN::SCREEN_WIDTH / 2 - 8, SCREEN::SCREEN_HEIGHT / 2 - 8);
  this->set_center(std::move(new_center));
  Vec2<double> new_thrust (0, 0);
  this->set_thrust(std::move(new_thrust));
  this->remove_health(1);
  this->calculate_pos(); // calculate the new triangle positions
  #ifdef DEBUG
  std::cout << "\nPlayer lives: " << this->lives << std::endl;
  #endif 
  this->set_invincible(true);
  #ifdef DEBUG
  std::cout << "Player is now invincible "<< std::endl;
  #endif 
}

void
Player::set_invincible(bool inv) {
  // reset clock
  this->clock = SDL_GetTicks();
  this->invincible = inv;
}

bool
Player::is_invincible(void) {
  auto ellapsed = SDL_GetTicks() - this->clock;
  if (this->invincible && ellapsed > INVINCIBILITY_TIME) {
    // reset clock
    this->clock = SDL_GetTicks();
    this->invincible = false;
    #ifdef DEBUG
    std::cout << "\nPlayer is not invincible anymore"<< std::endl;
    #endif 
  }
  return this->invincible;
}

void 
Player::update_score(int score) {
  this->score += score;
}

int
Player::update_score_texture(const std::shared_ptr<SDL_Renderer>& rend, std::shared_ptr<SDL_Texture>& texture) 
{ 
  if (this->score == this->old_score)
    return std::to_string(this->score).length() * NUMBER_WIDTH;

  // update old score
  this->old_score = this->score;

  if(texture == nullptr) {
    // max score = 999 999
    texture = SDL_wrapper::create_texture(rend, NUMBER_HEIGHT, 6*NUMBER_WIDTH);
  }
  
  // update our score texture
  return SDL_wrapper::update_score(rend, texture, this->score);
}