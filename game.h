#ifndef _GAME_H
#define _GAME_H
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <zlib.h>
#include <png.h>
#include "memory"

class Player 
{
  private:
    double x, y;
    SDL_Texture *texture;
    SDL_Surface *player_surf;
    SDL_Rect    *dst, *src;

  public:
    void           set_surf             (SDL_Surface *);
    void           set_tex              (SDL_Texture *);
    void           set_rect_src         (SDL_Rect *);
    void           set_rect_dst         (SDL_Rect *);
    SDL_Surface   *get_surf             (void);
    SDL_Texture   *get_tex              (void);
    SDL_Rect      *get_rect_dst         (void);  
    SDL_Rect      *get_rect_src         (void);
                   Player               (void);
                  
};

class Game 
{
  private:
    SDL_Window   *win;
    SDL_Surface  *surf;
    SDL_Renderer *rend;
    SDL_Event     ev;
    std::vector<std::shared_ptr<Player>> players;

  public:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    bool quit = false;
    
    void           load_tex             (std::shared_ptr<Player>, std::string, SDL_Rect *, SDL_Rect *);
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
