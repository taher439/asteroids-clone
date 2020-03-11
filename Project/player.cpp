#include "player.h"
#include "sdl_wrapper.h"

void 
Player::draw_ship(const std::shared_ptr<SDL_Renderer>& rend, const bool& thrusting)
{
    SDL_wrapper::draw_line(
        rend, 
        this->center.x + 10 * sin(this->angle),
        this->center.y - 10 * cos(this->angle), 
        this->center.x - 10 * (sin(this->angle) + cos(this->angle)),
        this->center.y + 10 * (cos(this->angle) - sin(this->angle)));
                           
    SDL_wrapper::draw_line(
        rend, 
        this->center.x + 10 * sin(this->angle),
        this->center.y - 10 * cos(this->angle), 
        this->center.x - 10 * (sin(this->angle) - cos(this->angle)),
        this->center.y + 10 * (cos(this->angle) + sin(this->angle)));

     SDL_wrapper::draw_line(
        rend,
        this->center.x - 7 * (sin(this->angle) + cos(this->angle)),
        this->center.y + 7 * (cos(this->angle) - sin(this->angle)),
        this->center.x - 7 * (sin(this->angle) - cos(this->angle)),
        this->center.y + 7 * (cos(this->angle) + sin(this->angle)));

    if (thrusting) {
        //add thrust animation
    }
}

void Player::add_blast(const double& angle)
{
  std::shared_ptr<blast> tmp = std::make_shared<blast>();
  tmp->loc = Vec2<double>(this->center);
  tmp->angle = angle;
  #ifdef DEBUG
    tmp->loc.print();
  #endif
  this->blasts.push_back(tmp);
}

void Player::draw_fire(const std::shared_ptr<SDL_Renderer>& rend)
{
    for (auto s = this->blasts.begin(); s != this->blasts.end(); ++s) {
        if ((*s)->loc.x > this->SCREEN_WIDTH) 
            s = this->blasts.erase(s);
        if ((*s)->loc.x < 0) 
            s = this->blasts.erase(s);            
        if ((*s)->loc.y > this->SCREEN_HEIGHT) 
            s = this->blasts.erase(s);   
        if ((*s)->loc.y < 0) 
            s = this->blasts.erase(s);
            
        if (s < this->blasts.end()) {
            (*s)->loc.x += cos((*s)->angle - 20.4) * 5;
            (*s)->loc.y += sin((*s)->angle - 20.4) * 5;
            SDL_wrapper::draw_point(rend, (*s)->loc.x, (*s)->loc.y);
            #ifdef DEBUG
                std::cout << "current angle " << angle << std::endl; 
            #endif
        } else break;
    }
}
