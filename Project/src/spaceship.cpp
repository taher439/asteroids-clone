#include <SDL.h>
#include <cmath>
#include "spaceship.h"
#include "player.h"
#include "sdl_wrapper.h"
#include "templates.h"
#include "collision.h"

Spaceship::Spaceship(const std::shared_ptr<SDL_Renderer>& rend, int height, int width, int shoot_freq) :
Moving_object(rend, height, width)
{
    this->health = 25;
    this->create_texture(height, width);
    this->clock = SDL_GetTicks();
    this->shoot_frequency = shoot_freq;
}

void 
Spaceship::detect_blast_collision(std::vector<std::shared_ptr<blast>>& blasts) 
{
  for (auto s = blasts.begin(); s != blasts.end(); ++s) {
    if (collision::point_in_ellipse((*s)->loc, this->center, this->width/2, this->height/2) && this->health >= 0) {
      this->health -= 25;
      blasts.erase(s);
      s--;
    }
  }
}

void
Spaceship::update(const std::vector<std::shared_ptr<Player>>& players)
{
  this->move();
  for (auto p: players) {
    this->detect_blast_collision(p->get_blasts());
    if (!this->is_alive()) {
      // if asteroid died, update player score and leave function
      p->update_score(this->get_score());
      return;
    }
  }
  this->detect_player_collision(players);
  this->detect_player_shot(players);

  // if alive and time to shoot then do it
  auto ellapsed = SDL_GetTicks() - this->clock;
  if (this->is_alive() && ellapsed > this->shoot_frequency) {
      this->shoot_players(players);
      this->clock = SDL_GetTicks();
  }
  this->draw_fire();
}

void
Spaceship::shoot_players(const std::vector<std::shared_ptr<Player>>& players)
{
  //shoot all players
  // #ifdef DEBUG
  //   std::cout << "\nAsteroid shot " << std::endl;
  // #endif 

  Vec2<double> pcenter;
  double angle;
  for (auto p: players) {
    pcenter = p->get_center();
    angle = atan2(pcenter.y - this->center.y , pcenter.x - this->center.x);
    this->add_blast(angle);
    // result = atan2 (y,x) * 180 / PI;
    // #ifdef DEBUG
    // std::cout << "\nAngle =  " << angle * 180 / PI << std::endl;
    // #endif 
  }
}

void
Spaceship::detect_player_shot (const std::vector<std::shared_ptr<Player>>& players)
{
  for (auto s = this->blasts.begin(); s != this->blasts.end(); ++s) {
    for (auto p: players) {
      if (collision::point_in_triangle((*s)->loc, p->A, p->B, p->D))
      {
        p->take_damage();
        s = this->blasts.erase(s);
        s--;
      }
    }
  }
 
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
    // SDL_SetRenderDrawColor(this->rend.get(), 0, 0, 255, 255);
    // SDL_RenderDrawRect(this->rend.get(), &drect);
    // draw red hitbox
    // SDL_SetRenderDrawColor(this->rend.get(), 255, 0, 0, 255);
    // SDL_wrapper::draw_circle(this->rend, texture_center.x, texture_center.y, this->height/2);

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
  // temporary test if point in circle
  return collision::line_circle(A, B, this->center, this->height/2);
  // TODO: test if point in elipse
}

void 
Spaceship::add_blast(double angle)
{
  std::shared_ptr<blast> tmp = std::make_shared<blast>();
  tmp->loc = Vec2<double>(this->center);
  tmp->angle = angle;
  this->blasts.emplace_back(tmp);
}

void 
Spaceship::draw_fire()
{
  // #ifdef DEBUG
  //  std::cout << "fire " << std::endl; 
  // #endif
    if (this->blasts.empty()) {
        return;
    }

    for (auto s: this->blasts) {
        s->loc.x += cos(s->angle) * this->shoot_speed;
        s->loc.y += sin(s->angle) * this->shoot_speed;
        SDL_wrapper::draw_point(this->rend, (int) s->loc.x, (int) s->loc.y);
    }

    for (auto s = this->blasts.begin(); s != this->blasts.end(); ++s) {
        if ((*s)->loc.x > SCREEN::SCREEN_WIDTH 
            || ((*s)->loc.x < 0) 
            || ((*s)->loc.y > SCREEN::SCREEN_HEIGHT) 
            || ((*s)->loc.y < 0)) {
            s = this->blasts.erase(s);
            s--;
        }
    }
}

int
Spaceship::get_score(void)
{
  return SPACESHIP_SCORE;
}