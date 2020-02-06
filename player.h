#include "game.h"
#include "sdl_wrapper.h"

class Player 
{
  private:
    double x, y, angle;
    std::shared_ptr<SDL_Texture> texture;
    std::shared_ptr<SDL_Surface> player_surf;
    std::shared_ptr<SDL_Rect>    dst, src;

  public:
    inline void           set_angle            (double);
    inline void           set_x                (double);
    inline void           set_y                (double);
    inline void           set_surf             (std::shared_ptr<SDL_Surface>);
    inline void           set_tex              (std::shared_ptr<SDL_Texture>);
    inline void           set_rect_src         (std::shared_ptr<SDL_Rect>);
    inline void           set_rect_dst         (std::shared_ptr<SDL_Rect>);
    inline std::shared_ptr<SDL_Surface>
                          get_surf             (void);

    inline std::shared_ptr<SDL_Texture>
                          get_tex              (void);

    inline std::shared_ptr<SDL_Rect>
                          get_rect_dst         (void);

    inline std::shared_ptr<SDL_Rect>
                          get_rect_src         (void);

    inline double         get_angle            (void);
    inline double         get_x                (void);
    inline double         get_y                (void);
                          Player               (void);
                  
};