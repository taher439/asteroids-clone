#include "game.h"

inline void 
ret_err_SDL(const std::string& err_msg) 
{
  std::cerr << err_msg << ": " << SDL_GetError() << std::endl;
  exit(EXIT_FAILURE);
}

Game::Game() 
{
  this->win = NULL;
  this->surf = NULL;
}

void 
Game::init(void) 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
      ret_err_SDL("SDL could not initialize! SDL_Error");

    this->win = SDL_CreateWindow("SDL Tutorial", 
                                  SDL_WINDOWPOS_UNDEFINED, 
                                  SDL_WINDOWPOS_UNDEFINED, 
                                  this->SCREEN_WIDTH, 
                                  this->SCREEN_HEIGHT, 
                                  SDL_WINDOW_SHOWN);

    if (this->win == NULL) 
      ret_err_SDL("Window could not be created! SDL_Error");

    this->surf = SDL_GetWindowSurface(this->win);
    
    SDL_FillRect(this->surf, NULL, 
                 SDL_MapRGB(this->surf->format, 
                            0xff, 0xff, 0xff));

    SDL_UpdateWindowSurface(this->win);
}

void 
Game::kill(void) 
{
    SDL_DestroyWindow(this->win);
    SDL_Quit();
}

SDL_Window *
Game::get_win(void) 
{
  return this->win;
}
