#include "player.h"
#include "game.h"
#include "sdl_wrapper.h"
#include "asteroid.h"

bool quit = false;

void 
Game::init(int asteroid_num, bool mp) 
{
  SDL_wrapper::init();
  this->mp = mp;
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
    center.x = Rand_gen<double>::rand_num(0, SCREEN_WIDTH);
    center.y = Rand_gen<double>::rand_num(0, SCREEN_HEIGHT);
    this->active_asteroids.push_back(std::make_shared<Asteroid>(20, center, BIG));
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

  int r = 0;
  Vec2<double> tmp_v;
  double size;
  std::list<int>  to_delete;

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
    
      p->set_angle(p->get_angle() + p->hdl.sprite_angle);

      if (p->hdl.thrust)
        p->thrust(p->get_angle());
      else
        p->slow_ship();
      if (p->hdl.blast) {

      #ifdef DEBUG
        std::cout << "player pressed fire button\n";
      #endif

        p->add_blast(p->get_angle());
        p->hdl.blast = false;
      }
      p->wrap_ship();
      p->move_ship();

      if (p->hdl.quit)
        quit = true;

        p->draw_ship(this->rend, p->hdl.thrust);
        p->draw_fire(this->rend);

        for (int i = 0; i < this->active_asteroids.size(); i++) {
          auto a = this->active_asteroids[i];
          if (a == nullptr) continue;
          a->detect_collision_ship(p->blasts);
          a->draw_asteroid(this->rend);
          p->asteroid_collision(a);
          if (a->get_health() == 0) {
            size = a->get_size();
            tmp_v = a->get_center();
            this->split_asteroid(tmp_v, size);
            to_delete.emplace_back(i);
            #ifdef DEBUG
            std::cout << "index to delete: " << r << std::endl;
            #endif
          }
          a->move_asteroid();
        }

        for (auto i: to_delete) {
          this->active_asteroids.erase(this->active_asteroids.begin() + i);
        }

        to_delete.clear();
    //framerate limit
      frame_time = SDL_GetTicks() - frame_start;
      if (frame_delay > frame_time) 
        SDL_Delay(frame_delay - frame_time);
    }
    SDL_RenderPresent(rend.get());
  }

  if (quit) {
  this->kill();
  }
}

int
Game::split_asteroid(Vec2<double> v, double current_size)
{
  if (current_size == BIG) {
    this->active_asteroids.push_back(std::make_shared<Asteroid>(20, v + BIG, MID));
    this->active_asteroids.push_back(std::make_shared<Asteroid>(20, v - BIG, MID));
  }
  else if (current_size == MID) {
    this->active_asteroids.push_back(std::make_shared<Asteroid>(5, v + MID, SMALL));
    this->active_asteroids.push_back(std::make_shared<Asteroid>(5, v - MID, SMALL));
    this->active_asteroids.push_back(std::make_shared<Asteroid>(5, v - MID, SMALL));
    this->active_asteroids.push_back(std::make_shared<Asteroid>(5, v + MID, SMALL));
  }
  return 0;
}