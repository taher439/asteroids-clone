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
#include <cmath>
#define PI 3.14159265

class Game 
{
  private:
    std::shared_ptr<SDL_Window>   win;
    std::shared_ptr<SDL_Surface>  surf;
    std::shared_ptr<SDL_Renderer> rend;

    SDL_Event ev;
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Player> main_player;

    enum state {
      MENU,
      GAME,
      CONFIG
    };

    state current_state;
    
  public:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    bool quit = false;
    
    void                  load_tex             (std::shared_ptr<Player>,
                                                std::string, 
                                                SDL_Rect *, 
                                                SDL_Rect *);

    void                  kill                 (void);
    void                  proc_input           (void);
    void                  init                 (void);
    inline void           set_win              (std::shared_ptr<SDL_Window>);
    inline void           set_surf             (std::shared_ptr<SDL_Surface>);
    inline void           set_rend             (std::shared_ptr<SDL_Renderer>);
    inline std::shared_ptr<SDL_Renderer> 
                          get_rend             (void);

    inline std::shared_ptr<SDL_Surface>
                          get_surf             (void);

    inline std::shared_ptr<SDL_Window>
                          get_win              (void);
                          Game                 (void);
    
    inline std::shared_ptr<Player> 
                         get_main_player       (void);
    inline void          set_main_player       (std::shared_ptr<Player>);
};

class SDL_Wrapper {
  
}
#endif 
