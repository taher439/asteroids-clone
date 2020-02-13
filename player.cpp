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