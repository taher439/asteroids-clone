#ifndef _GAME_H
#define _GAME_H
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "memory"

class Game 
{
  private:
    SDL_Window *win;
    SDL_Surface *surf;

  public:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    

    void           init                 (void);
    void           set_win              (SDL_Window *);
    void           set_surf             (SDL_Surface *);
    SDL_Surface   *get_surf             (void);
    SDL_Window    *get_win              (void);
                   Game                 (void);
    void           kill                 (void);
};

#endif 
