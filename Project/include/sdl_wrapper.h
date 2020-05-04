#ifndef _SDL_WRAPPER_H
#define _SDL_WRAPPER_H
#include <map>
#include "game.h"
#define MAX_DIGITS 6
#define NUMBER_WIDTH 16
#define NUMBER_HEIGHT 25
#define LIFE_WIDTH 25
#define LIFE_HEIGHT 25

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
        static void       ret_err_SDL      (const std::string&);
        static std::shared_ptr<SDL_Window>       
                          creat_win        (const int&, const int&);
        static std::shared_ptr<SDL_Renderer>       
                          creat_rend       (const std::shared_ptr<SDL_Window>&);
        static std::shared_ptr<SDL_Texture>
                          create_texture   (const std::shared_ptr<SDL_Renderer>&, int, int);
        static void       draw_color       (const std::shared_ptr<SDL_Renderer>&);
        static void       rend_copy_ex     (const std::shared_ptr<SDL_Renderer>&, 
                                            const std::shared_ptr<Player>&);
        static void       rend_clear       (const std::shared_ptr<SDL_Renderer>&);
        static void       draw_line        (const std::shared_ptr<SDL_Renderer>&,
                                            const int&,
                                            const int&,
                                            const int&,
                                            const int&);

        static void       draw_lines       (const std::shared_ptr<SDL_Renderer>&,
                                            const std::vector<SDL_Point>&);
        static void       draw_point       (const std::shared_ptr<SDL_Renderer>&, 
                                            const int&, 
                                            const int&);
                                            
        static void       fill_screen      (const std::shared_ptr<SDL_Window>&);
        inline static SDL_Surface* load_surface   (std::string const path) {return load_surface(path.c_str());};
        static SDL_Surface* load_surface   (char const *);
        static std::shared_ptr<SDL_Texture>
                          load_texture     (const std::shared_ptr<SDL_Renderer>&, std::string const);
        static void draw_circle            (const std::shared_ptr<SDL_Renderer>&, int32_t, int32_t, int32_t);
        static void load_textures          (const std::shared_ptr<SDL_Renderer>&);
        static int  update_score           (const std::shared_ptr<SDL_Renderer>&, std::shared_ptr<SDL_Texture>, const int);
        static int  update_life            (const std::shared_ptr<SDL_Renderer>&, std::shared_ptr<SDL_Texture>, const int);
        
        
        
        static std::map<char, std::shared_ptr<SDL_Texture>> numbers;
        static std::shared_ptr<SDL_Texture> life;
        static std::shared_ptr<SDL_Texture> dead;
};

#endif