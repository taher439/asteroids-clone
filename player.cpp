#include "player.h"
#include "sdl_wrapper.h"

void 
Player::draw_ship(std::shared_ptr<SDL_Renderer> rend) 
{
    SDL_wrapper::draw_line(rend, 
                           this->x + 10 * sin(this->angle),
                           this->y - 10 * cos(this->angle), 
                           this->x - 10 * (sin(this->angle) + cos(this->angle)),
                           this->y + 10 * (cos(this->angle) - sin(this->angle)));
                           
    SDL_wrapper::draw_line(rend, 
                           this->x + 10 * sin(this->angle),
                           this->y - 10 * cos(this->angle), 
                           this->x - 10 * (sin(this->angle) - cos(this->angle)),
                           this->y + 10 * (cos(this->angle) + sin(this->angle)));
}