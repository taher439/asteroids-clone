#include "player.h"
#include "sdl_wrapper.h"
#include "asteroid.h"

void 
Player::draw_ship(const std::shared_ptr<SDL_Renderer>& rend, const bool& thrusting)
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
        s->loc.x += cos(s->angle - 20.45) * 5;
        s->loc.y += sin(s->angle - 20.45) * 5;
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
 
void 
Player::asteroid_collision(const std::shared_ptr<Asteroid>& a)
{
  auto ccw   = [](Vec2<double>& A, Vec2<double>& B, Vec2<double>& C) 
                {return (C.y - A.y) * (B.x - A.x) < (B.y-A.y) * (C.x-A.x);};
  auto inter = [&](Vec2<double>& A, Vec2<double>& B, Vec2<double>& C, Vec2<double>& D)
                {ccw(A,C,D) != ccw(B,C,D) and ccw(A,B,C) != ccw(A,B,D);};

  if (a->detect_inter(this->A, this->B) || 
      a->detect_inter(this->C, this->D) || 
      a->detect_inter(this->E, this->F)) {
        this->center.x = SCREEN_WIDTH / 2 - 8;
        this->center.y = SCREEN_HEIGHT / 2 - 8;
        this->thrust_vec.x = 0;
        this->thrust_vec.y = 0;
        this->lives -= 1;
#ifdef DEBUG
        std::cout << "\nplayer lives: " << this->lives << std::endl;
#endif 
    }
}