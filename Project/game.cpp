#include "player.h"
#include "game.h"
#include "sdl_wrapper.h"
#include "asteroid.h"


void 
Game::init(int&& asteroid_num) 
{
  SDL_wrapper::init();
  this->win = SDL_wrapper::creat_win(this->SCREEN_WIDTH, this->SCREEN_HEIGHT);
  this->rend = SDL_wrapper::creat_rend(this->win);
  //initialize the asteroid field
  this->total_asteroids = std::move(asteroid_num);
  for (int i = 0; i < this->total_asteroids; i++)
    this->active_asteroids.push_back(std::make_shared<Asteroid>(5, 100, 100, SMALL));
}

void 
Game::kill(void) 
{
  SDL_Quit();
}

static void 
handle_event(sdl_event_handler& hdl) 
{
  switch(hdl.ev.type) {
        case SDL_QUIT:
          #ifdef DEBUG
            std::cout << "player pressed quit\n";
          #endif
          hdl.quit = true;
          break;
        case SDL_KEYDOWN: {
          if (hdl.ev.key.keysym.sym == LEFT)
            hdl.sprite_angle = (-360.0000 / 180.0000) * PI / hdl.FPS;
          if (hdl.ev.key.keysym.sym == RIGHT)
            hdl.sprite_angle = (360.0000  / 180.0000) * PI / hdl.FPS;
          
          if (hdl.ev.key.keysym.sym == SDLK_SPACE)
            hdl.blast = true;

          break;
        }
        case SDL_KEYUP: {
          if (hdl.ev.key.keysym.sym == LEFT || 
              hdl.ev.key.keysym.sym == RIGHT)
            hdl.sprite_angle = 0;

          if (hdl.ev.key.keysym.sym == SDLK_SPACE)
            hdl.blast = false;
          break;
        }
        case SDL_MOUSEBUTTONDOWN: {
          if (hdl.ev.button.button == SDL_BUTTON_LEFT)
            hdl.thrust = true;
          break;
        }
        case SDL_MOUSEBUTTONUP: {
          if (hdl.ev.button.button == SDL_BUTTON_LEFT)
            hdl.thrust = false;
          break;
        }
   }
}

void 
Game::proc_input(void) 
{
  sdl_event_handler hdl;
  hdl.sprite_angle = 0;
  hdl.thrust = false;
  hdl.ev = this->ev;
  hdl.FPS = 60;
  hdl.quit = false;
  hdl.blast = false;
  const int frame_delay = 1000 / hdl.FPS;
  Uint32    frame_start;
        int frame_time;
  
  while(!hdl.quit) { 
    frame_start = SDL_GetTicks();
    while(SDL_PollEvent(&(hdl.ev)) != 0)
      handle_event(hdl);

    main_player->set_angle(main_player->get_angle() + hdl.sprite_angle);
    if (hdl.thrust)
      main_player->thrust(main_player->get_angle());
    else
      main_player->slow_ship();
    
    if (hdl.blast) {
      #ifdef DEBUG
        std::cout << "player pressed fire button\n";
      #endif
      main_player->add_blast(main_player->get_angle());
      hdl.blast = false;
    }

    main_player->wrap_ship();
    main_player->move_ship();

    SDL_SetRenderDrawColor(rend.get(), 0, 0, 0, 255);
    SDL_RenderClear(rend.get());
      main_player->draw_ship(this->rend, hdl.thrust);

      for (auto a: this->active_asteroids) {
        a->draw_asteroid(this->rend);
        a->detect_collision_ship(main_player->blasts);
      }
      main_player->draw_fire(this->rend);
    SDL_RenderPresent(rend.get());
  
    //framerate limit
    frame_time = SDL_GetTicks() - frame_start;
    if (frame_delay > frame_time) 
      SDL_Delay(frame_delay - frame_time);
  }
  
  if (hdl.quit)
    this->kill();
}