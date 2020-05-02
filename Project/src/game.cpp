#include "player.h"
#include "game.h"
#include "particle.h"
#include "sdl_wrapper.h"
#include "asteroid.h"
#include "spaceship.h"

bool quit = false;

void 
Game::init(int asteroid_num, bool mp) 
{
  SDL_wrapper::init();
  this->mp = mp;
  this->current_level = 1;
  this->win = SDL_wrapper::creat_win(this->SCREEN_WIDTH, this->SCREEN_HEIGHT);
  this->rend = SDL_wrapper::creat_rend(this->win);
  //initialize the asteroid field
  this->total_asteroids = asteroid_num;
  Vec2<double> center(0, 0);

  if (mp) {
    std::shared_ptr<Player> sec_p = std::make_shared<Player>(700, 480);
    sec_p->hdl.second_p = true;
    this->players.push_back(sec_p);
  }

  this->players.push_back(std::make_shared<Player>(640, 480));

  for (int i = 0; i < this->total_asteroids; i++) {
    // this->moving_objects.emplace_back(std::make_shared<Asteroid>(this->rend, 20, center, 2*BIG, 2*BIG));
    this->moving_objects.emplace_back(std::make_shared<Spaceship>(this->rend, 100, 100, 60, 123));
    this->moving_objects.front()->set_speed(0);
  }
}

void 
Game::kill(void) 
{
  SDL_Quit();
}

static void 
handle_event(sdl_event_handler& hdl) 
{
  switch(hdl.ev->type) {
        case SDL_QUIT:
          #ifdef DEBUG
            std::cout << "player pressed quit\n";
          #endif
          hdl.quit = true;
          break;
        case SDL_KEYDOWN: {
          if (hdl.ev->key.keysym.sym == LEFT && !hdl.second_p) {
            #ifdef DEBUG
               std::cout << "\033[1;31mbold [*] left key pressed\033[0m\n" << std::endl;
            #endif
            hdl.sprite_angle = (-360.0000 / 180.0000) * PI / hdl.FPS;
          }
            
          if (hdl.ev->key.keysym.sym == RIGHT && !hdl.second_p) {
             #ifdef DEBUG
              std::cout << "\033[1;31mbold [*] right key pressed\033[0m\n" << std::endl;
            #endif
            hdl.sprite_angle = (360.0000  / 180.0000) * PI / hdl.FPS;
          }
          if (hdl.ev->key.keysym.sym == LEFT_ALT && hdl.second_p) {
             #ifdef DEBUG
              std::cout << "\033[1;31mbold [*] left alt key pressed\033[0m\n" << std::endl;
            #endif
            hdl.sprite_angle = (-360.0000 / 180.0000) * PI / hdl.FPS;
          }

          if (hdl.ev->key.keysym.sym == RIGHT_ALT && hdl.second_p) {
             #ifdef DEBUG
              std::cout << "\033[1;31mbold [*] right alt key pressed\033[0m\n" << std::endl;
            #endif
            hdl.sprite_angle = (360.0000  / 180.0000) * PI / hdl.FPS;
          }

          if (hdl.ev->key.keysym.sym == SDLK_SPACE && !hdl.second_p) {
            hdl.blast = true;
          }

          if (hdl.ev->key.keysym.sym == SDLK_BACKSPACE && hdl.second_p) {
            hdl.blast = true;
          }

          if (hdl.ev->key.keysym.sym == SDLK_DOWN && hdl.second_p) {
            hdl.thrust = true;
          }
          break;
        }
        case SDL_KEYUP: {
          if ((hdl.ev->key.keysym.sym == LEFT || 
               hdl.ev->key.keysym.sym == RIGHT) && !hdl.second_p)
            hdl.sprite_angle = 0;

          if ((hdl.ev->key.keysym.sym == LEFT_ALT || 
               hdl.ev->key.keysym.sym == RIGHT_ALT) && hdl.second_p)
            hdl.sprite_angle = 0;

          if (hdl.ev->key.keysym.sym == SDLK_SPACE && !hdl.second_p)
            hdl.blast = false;

          if (hdl.ev->key.keysym.sym == SDLK_BACKSPACE && hdl.second_p)
            hdl.blast = false;
          
          if (hdl.ev->key.keysym.sym == SDLK_DOWN && hdl.second_p)
            hdl.thrust = false;
          break;
        }
        case SDL_MOUSEBUTTONDOWN: {
          if (hdl.ev->button.button == SDL_BUTTON_LEFT && !hdl.second_p)
            hdl.thrust = true;
          break;
        }
        case SDL_MOUSEBUTTONUP: {
          if (hdl.ev->button.button == SDL_BUTTON_LEFT && !hdl.second_p)
            hdl.thrust = false;
          break;
        }
   }
}


void
handle_event_task(const std::shared_ptr<Player>& p) {
  while (!quit) {
    while(SDL_PollEvent(p->hdl.ev.get()) != 0)
      handle_event(p->hdl);
  }
}

void 
Game::proc_input(void) 
{
  const int frame_delay = 1000 / 60;
  Uint32    frame_start;
        int frame_time;
  this->ev = std::make_shared<SDL_Event>();
  this->players[0]->hdl.ev = this->ev;
  if (this->mp) {
    //p2 = std::thread(handle_event_task, this->players[1]);
    this->players[1]->hdl.ev = this->ev;
  }

  Vec2<double> center(0, 0);
  int r = 0;
  Vec2<double> tmp_v;
  double size;
  std::list<int>  to_delete;
  std::list<int>  dead_player;
  int player_index = 0;
  while(!quit) { 
    frame_start = SDL_GetTicks();
    SDL_SetRenderDrawColor(rend.get(), 0, 0, 0, 255);
    SDL_RenderClear(rend.get());

    while(SDL_PollEvent(this->ev.get()) != 0) {
      handle_event(this->players[0]->hdl);
      if (mp)
        handle_event(this->players[1]->hdl);
    }

    for (auto p: this->players) {
      if (p->get_health() <= 0) {
        dead_player.emplace_back(player_index);
        continue;
      }
      player_index++;
      p->set_angle(p->get_angle() + p->hdl.sprite_angle);

      if (p->hdl.thrust)
        p->thrust(p->get_angle());
      else
        p->slow_ship();
      if (p->hdl.blast) {

      #ifdef DEBUG
        std::cout << "player pressed fire button\n";
      #endif
        p->add_blast();
        p->hdl.blast = false;
      }

      p->wrap_ship();
      p->move_ship();

      if (p->hdl.quit)
        quit = true;

      p->draw_ship(this->rend, p->hdl.thrust);
      p->draw_fire(this->rend);
      
      // update moving objects => move them, detect blast collision
      this->update_objects();
      this->particles();
      
    //   for (int i = 0; i < this->active_asteroids.size(); i++) 
    //   for (auto i = this->active_asteroids.begin(); i != this->active_asteroids.end(); i++)
    //   {
    //     auto a = (*i);
    //     if (a == nullptr) continue;
    //     a->detect_collision(p->blasts);
    //     p->asteroid_collision(a);
    //     // a->draw_asteroid(); // kept for retro
    //     a->draw();

    //     if (a->get_health() == 0) {
    //       size = a->get_size();
    //       tmp_v = a->get_center();
    //       this->particle_clouds.emplace_back(this->generate_particles(tmp_v, 50));
    //       this->split_asteroid(tmp_v, size);
    //       // to_delete.emplace_back(i);
    //       i = this->active_asteroids.erase(i);
    //       i--;
    //       #ifdef DEBUG
    //       std::cout << "index to delete: " << r << std::endl;
    //       #endif
    //     }
    //     a->update(this->players[0]->blasts);
    //   }

      // for (auto i: to_delete) {         
      //   // this->active_asteroids.erase(this->active_asteroids.begin() + i);
      //   this->active_asteroids.erase(i);
      // }

      
      // to_delete.clear();
    //framerate limit
      frame_time = SDL_GetTicks() - frame_start;
      if (frame_delay > frame_time) 
        SDL_Delay(frame_delay - frame_time);
    }

    //proceed to next level

    // if (this->active_asteroids.empty()) {
    if (this->moving_objects.empty()) {
      this->current_level++;
      this->total_asteroids *= this->current_level; 
      for (int i = 0; i < this->total_asteroids; i++) {
        this->moving_objects.emplace_back(std::make_shared<Asteroid>(this->rend, 5, 2*BIG, 2*BIG));
        this->moving_objects.emplace_back(std::make_shared<Spaceship>(this->rend, 20, 41));
      }
    }

    for (auto i: dead_player) {
        this->players.erase(this->players.begin() + i);
        if (this->players.empty()) {
          #ifdef DEBUG
            std::cout << "GAME OVER\n";
          #endif 
          quit = true;
          break;
        }
    }
    player_index = 0;
    dead_player.clear();
    SDL_RenderPresent(rend.get());
    SDL_RenderClear(rend.get());
  }

  if (quit) {
#ifdef DEBUG
    std::cout << "killing SDL\n";
#endif
    this->kill();
  }
}

int
Game::split_asteroid(Vec2<double> v, double current_size)
{
  if (current_size == BIG) {
    this->moving_objects.emplace_back(std::make_shared<Asteroid>(rend, 20, v + BIG, 2*MID, 2*MID));
  }
  else if (current_size == MID) {
    this->moving_objects.emplace_back(std::make_shared<Asteroid>(this->rend, 5, v + MID, 2*SMALL, 2*SMALL));
    this->moving_objects.emplace_back(std::make_shared<Asteroid>(this->rend, 5, v - MID, 2*SMALL, 2*SMALL));
  }
  return 0;
}

std::shared_ptr<Particle_container>
Game::generate_particles (Vec2<double> pos, int count)
{
  auto pc = std::make_shared<Particle_container>();
  for (int i = 0; i < count; i++)
  {
    auto p = std::make_shared<Ephemeral_particle>(pos, 30, 50);
    pc->add_particle(p);
  }
  return pc;
}

void
Game::particles(void)
{
  for (auto pc = this->particle_clouds.begin(); pc != this->particle_clouds.end(); pc++)
  {
    (*pc)->step_all();
    (*pc)->draw_all(this->rend);
    (*pc)->clean_all();
    if ( (*pc)->is_empty() )
    { 
      pc = this->particle_clouds.erase(pc);
      pc--;
    }
  }
}

void
Game::update_objects(void)
{
  Vec2<double> tmp_v;
  double height;
  for (auto mo = this->moving_objects.begin(); mo != this->moving_objects.end(); mo++)
  {
    // TODO : liste générale de blasts mais identifiant du player pour un blast
    (*mo)->update(this->players[0]->blasts, this->players[0]);

    // if object died, generate particles
    if ( !(*mo)->is_alive() )
    { 
      height = (*mo)->get_height();
      tmp_v = (*mo)->get_center();
      this->particle_clouds.emplace_back(this->generate_particles(tmp_v, 50));
      // if the object was an asteroid, split it
      if ( (*mo)->has_type("ASTEROID") ) {
        this->split_asteroid(tmp_v, height/2);
      }

      mo = this->moving_objects.erase(mo);
      mo--;
    } 
    else 
    {
      // after moving detect collision
      // this->players[0]->asteroid_collision((*mo));
      // then draw
      (*mo)->draw();
    }

  }
}