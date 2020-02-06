#include "game.h"

static void SDL_DelRes(SDL_Window   *r) { SDL_DestroyWindow(r);   }
static void SDL_DelRes(SDL_Renderer *r) { SDL_DestroyRenderer(r); }
static void SDL_DelRes(SDL_Texture  *r) { SDL_DestroyTexture(r);  }
static void SDL_DelRes(SDL_Surface  *r) { SDL_FreeSurface(r);     }

template <typename T>
std::shared_ptr<T> sdl_shared(T *t) {
    return std::shared_ptr<T>(t, [](T *t) { SDL_DelRes(t); });
}

class SDL_wrapper
{
    inline SDL_wrapper(void) = default;

    static void       init             (void);
    std::shared_ptr<SDL_Window>       
                      creat_win        (const int&, const int&);
    std::shared_ptr<SDL_Renderer>       
                      creat_rend       (const std::shared_ptr<SDL_Window>);
    static void       draw_color       (const std::shared_ptr<SDL_Renderer>);
    static void       rend_clear       (const std::shared_ptr<SDL_Renderer>);
    static void       rend_copy_ex     (const std::shared_ptr<SDL_Renderer>);
}