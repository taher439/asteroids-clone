#include "player.h"
#include "game.h"
#include "sdl_wrapper.h"

Game::Game() 
{
  this->win = NULL;
  this->surf = NULL;
}

void 
Game::init(void) 
{
  SDL_wrapper::init();
  this->win = SDL_wrapper::creat_win(this->SCREEN_WIDTH, this->SCREEN_HEIGHT);
  this->rend = SDL_wrapper::creat_rend(this->win);
}

void 
Game::kill(void) 
{
  SDL_Quit();
}

void Game::proc_input(void) 
{
  double sprite_angle = 0;
  bool thrust = false;

  const int FPS = 60;
  const int frame_delay = 1000/FPS;
  Uint32 frame_start;
  int frame_time;
  
  while(!this->quit) { 
    frame_start = SDL_GetTicks();

    while(SDL_PollEvent(&this->ev) != 0) {       
      switch(this->ev.type) {
        case SDL_QUIT:
          this->quit = true;
          break;

        case SDL_KEYDOWN:
        {
          if (this->ev.key.keysym.sym == LEFT)
            sprite_angle = (-360 / 180.0000) * PI / FPS;
           if (this->ev.key.keysym.sym == RIGHT)
            sprite_angle = (360 / 180.0000) * PI / FPS;
          break;
        }

        case SDL_KEYUP: {
          if (this->ev.key.keysym.sym == LEFT || this->ev.key.keysym.sym == RIGHT)
            sprite_angle = 0;
          break;
        }
        
        case SDL_MOUSEBUTTONDOWN:
        {
          if (this->ev.button.button == SDL_BUTTON_LEFT)
            thrust = true;
          break;
        }

        case SDL_MOUSEBUTTONUP:
        {
          if (this->ev.button.button == SDL_BUTTON_LEFT)
            thrust = false;
          break;
        }
      }
    }
    
    main_player->set_angle(main_player->get_angle() + sprite_angle);
    
    if (thrust)
      main_player->thrust(main_player->get_angle());
    else
      main_player->slow_ship();

    main_player->wrap_ship();
    main_player->move_ship();
    SDL_SetRenderDrawColor(rend.get(), 0, 0, 0, 255);
    SDL_RenderClear(rend.get());
      main_player->draw_ship(this->rend);
    SDL_RenderPresent(rend.get());
  
    frame_time = SDL_GetTicks() - frame_start;
    if (frame_delay > frame_time) 
      SDL_Delay(frame_delay - frame_time);
  }
  
  if (this->quit)
    this->kill();
}

void 
Game::load_tex(std::shared_ptr<Player> player, 
               std::string path, 
               std::shared_ptr<SDL_Rect> src, 
               std::shared_ptr<SDL_Rect> dst)
{
  auto tmp_surf = sdl_shared(IMG_Load(path.c_str()));
  player->set_surf(tmp_surf);
  player->set_rect_dst(dst);
  player->set_rect_src(src);
  if(player->get_surf() == NULL)
    std::cerr 
         << "Unable to load image, SDL_image Error " 
         << path.c_str() << " " 
         << IMG_GetError() 
         << std::endl;
  else
  {
    auto tmp_tex = sdl_shared(SDL_CreateTextureFromSurface(
                                this->rend.get(), 
                                player->get_surf().get()));

    player->set_tex(tmp_tex);
    if (player->get_tex() == NULL)
      std::cerr 
           << "Unable to create texture from %s! SDL Error: "
           << path.c_str() << " "
           << SDL_GetError() 
           << std::endl;
    this->players.push_back(player);
  }
}
