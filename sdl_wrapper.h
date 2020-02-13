#ifndef _SDL_WRAPPER_H
#define _SDL_WRAPPER_H
#include "game.h"

[[maybe_unused]] static void SDL_DelRes(SDL_Window *r) 
    {SDL_DestroyWindow(r);}
[[maybe_unused]] static void SDL_DelRes(SDL_Renderer *r) 
    {SDL_DestroyRenderer(r);}
[[maybe_unused]] static void SDL_DelRes(SDL_Texture *r) 
    {SDL_DestroyTexture(r);}
[[maybe_unused]] static void SDL_DelRes(SDL_Surface *r) 
    {SDL_FreeSurface(r);}

template <typename T>
std::shared_ptr<T> sdl_shared(T *t) {
    return std::shared_ptr<T>(t, [](T *t) {SDL_DelRes(t);});
}

class SDL_wrapper
{
    public:
        inline SDL_wrapper(void) = default;

        static void       init             (void);
        static std::shared_ptr<SDL_Window>       
                          creat_win        (const int&, const int&);
        static std::shared_ptr<SDL_Renderer>       
                          creat_rend       (const std::shared_ptr<SDL_Window>);
        static void       draw_color       (const std::shared_ptr<SDL_Renderer>);
        static void       rend_copy_ex     (const std::shared_ptr<SDL_Renderer>, 
                                            const std::shared_ptr<Player>);
        static void       rend_clear       (const std::shared_ptr<SDL_Renderer>);
        static void       draw_line        (const std::shared_ptr<SDL_Renderer>,
                                            const int&,
                                            const int&,
                                            const int&,
                                            const int&);

        static void       draw_lines       (const std::shared_ptr<SDL_Renderer>,
                                            const std::vector<SDL_Point>);

        static void       fill_screen      (const std::shared_ptr<SDL_Window>);
};

#endif