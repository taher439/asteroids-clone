#ifndef _GAME_H
#define _GAME_H
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "memory"

class Player 
{
  private:
    double x, y;
    SDL_Texture *texture;
    SDL_Surface *player_surf;

  public:
    void           set_surf             (SDL_Surface *);
    void           set_tex              (SDL_Texture *);
    SDL_Surface   *get_surf             (void);
    SDL_Texture   *get_tex              (void);
                   Player               (void);
                  
};

class Game 
{
  private:
    SDL_Window *win;
    SDL_Surface *surf;
    SDL_Renderer *rend;
    bool quit = false;
    SDL_Event ev;
    std::vector<std::unique_ptr<Player>> players;

  public:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    
    void           load_tex             (std::unique_ptr<Player>, std::string);
    void           kill                 (void);
    void           proc_input           (void);
    void           init                 (void);
    void           set_win              (SDL_Window *);
    void           set_surf             (SDL_Surface *);
    void           set_rend             (SDL_Renderer *);
    SDL_Renderer  *get_rend             (void);
    SDL_Surface   *get_surf             (void);
    SDL_Window    *get_win              (void);
                   Game                 (void);
};

#endif 
