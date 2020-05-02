#include <SDL.h>
#include "spaceship.h"
#include "player.h"
#include "sdl_wrapper.h"
#include "templates.h"

Spaceship::Spaceship(const std::shared_ptr<SDL_Renderer>& rend, int height, int width) :
Moving_object(rend, height, width)
{
    this->health = 25;
    // spaceship image dimensions
    this->create_texture(height, width);
}

void 
Spaceship::detect_blast_collision(std::vector<std::shared_ptr<blast>>& blasts) 
{
  double dx, dy, dist; 
  // calculate ellipse radius
  double rx2 = (this->width/2) * (this->width/2);
  double ry2 = (this->height/2) * (this->height/2);

  auto ellipse = [&](double x, double y) { return ry2*x*x + rx2*y*y - rx2*ry2;};

  for (auto s = blasts.begin(); s != blasts.end(); ++s) {
    dx = abs(this->center.x - (*s)->loc.x);
    dy = abs(this->center.y - (*s)->loc.y);
    dist = ellipse(dx, dy);
    if (dist <= 0 ) {
      #ifdef DEBUG
        std::cout << "\033[1;31mbold [*] collision detected\033[0m\n" << std::endl;
      #endif
      if (this->health >= 0)
        this->health -= 25;
      blasts.erase(s);
      s--;
    }
  }
}

void
Spaceship::update(std::vector<std::shared_ptr<blast>>& blasts, const std::shared_ptr<Player>& players)
{
  this->move();
  this->detect_blast_collision(blasts);
  this->detect_player_collision(players);
  // TODO : if alive and time to shoot then do it
  if (this->is_alive() && true) {
      this->shoot_players();
  }
}

void
Spaceship::shoot_players(void)
{
  //shoot all players
}

void 
Spaceship::create_texture (int height, int width)
{      
    this->texture = SDL_wrapper::create_texture(rend, height, width);
    SDL_SetTextureBlendMode(this->texture.get(), SDL_BLENDMODE_BLEND);
    // change renderer target
    SDL_SetRenderTarget(this->rend.get(), this->texture.get());

    // draw auxtexture (generatred from the image) on the texture
    std::shared_ptr<SDL_Texture> auxtexture = SDL_wrapper::load_texture(this->rend, DATA_PATH+"spaceship.bmp");
    SDL_Rect drect;
    drect.x = 0; drect.y = 0; drect.h = height; drect.w = width;
    SDL_RenderCopy(this->rend.get(), auxtexture.get(), NULL, &drect);

    Vec2<double> texture_center = this->center - this->top_left;
    // draw blue texture box
    SDL_SetRenderDrawColor(this->rend.get(), 0, 0, 255, 255);
    SDL_RenderDrawRect(this->rend.get(), &drect);
    // draw red hitbox
    SDL_SetRenderDrawColor(this->rend.get(), 255, 0, 0, 255);
    SDL_wrapper::draw_circle(this->rend, texture_center.x, texture_center.y, this->height/2);

    // reset the renderer
    SDL_SetRenderTarget(this->rend.get(), nullptr);
}

bool   
Spaceship::has_type (std::string str)
{
    if (str == "SPACESHIP") {
        return true;
    } else {
        return false;
    }
}

bool 
Spaceship::detect_inter(const Vec2<double>& A, const Vec2<double>& B)
{

//   for (auto i: this->points)    
//     if (Vec2<double>::dist(A, B, i+this->center))
//     return true;
  // TODO: test if point in elipse
  return false;
}

void 
Spaceship::detect_player_collision(const std::shared_ptr<Player>& p)
{
  auto ccw   = [](Vec2<double>& A, Vec2<double>& B, Vec2<double>& C) 
                {return (C.y - A.y) * (B.x - A.x) < (B.y-A.y) * (C.x-A.x);};
  auto inter = [&](Vec2<double>& A, Vec2<double>& B, Vec2<double>& C, Vec2<double>& D)
                {ccw(A,C,D) != ccw(B,C,D) and ccw(A,B,C) != ccw(A,B,D);};

  if (this->detect_inter(p->A, p->B) || 
      this->detect_inter(p->C, p->D) || 
      this->detect_inter(p->E, p->F)) 
  {
    Vec2<double> new_center (SCREEN::SCREEN_WIDTH / 2 - 8, SCREEN::SCREEN_HEIGHT / 2 - 8);
    p->set_center(std::move(new_center));
    Vec2<double> new_thrust (0, 0);
    p->set_thrust(std::move(new_thrust));
    p->remove_health(1);
    #ifdef DEBUG
      std::cout << "\nplayer lives: " << p->get_health() << std::endl;
    #endif 
  }
}