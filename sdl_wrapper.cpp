#include "sdl_wrapper.h"
#include "game.h"

static inline void 
ret_err_SDL(const std::string& err_msg) 
{
  std::cerr << err_msg << ": " << SDL_GetError() << std::endl;
  exit(EXIT_FAILURE);
}

void
SDL_wrapper::init(void) 
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    ret_err_SDL("SDL could not initialize! SDL_Error");

  if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    std::cerr << "Warning: Linear texture filtering not enabled!\n";
}

std::shared_ptr<SDL_Window>       
SDL_wrapper::creat_win(const int& width, const int& height) 
{
  std::shared_ptr<SDL_Window> win = 
  sdl_shared(SDL_CreateWindow("SDL Tutorial", 
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED, 
    width, 
    height, 
    SDL_WINDOW_SHOWN));

  if (win == nullptr) 
    ret_err_SDL("Window could not be created! SDL_Error");

  return win;
}

std::shared_ptr<SDL_Renderer>       
SDL_wrapper::creat_rend(const std::shared_ptr<SDL_Window> win) 
{
  std::shared_ptr<SDL_Renderer> rend = 
    sdl_shared(SDL_CreateRenderer(win.get(), -1, SDL_RENDERER_ACCELERATED));
  
  if (rend == nullptr) 
    ret_err_SDL("Renderer could not be created! SDL_Error");
  
  SDL_SetRenderDrawColor(rend.get(), 0xff, 0xff, 0xff, 0xff);
  int imgFlags = IMG_INIT_PNG;
  if(!(IMG_Init(imgFlags) & imgFlags))
    std::cerr 
      << "SDL_image could not initialize! SDL_image Error: " 
      << IMG_GetError() 
      << std::endl;
  return rend;
}

void 
SDL_wrapper::rend_copy_ex(const std::shared_ptr<SDL_Renderer> rend, 
                          const std::shared_ptr<Player> p) 
{
  int ret;
  ret = SDL_RenderCopyEx(rend.get(), 
                   p->get_tex(), 
                   p->get_rect_src(), 
                   p->get_rect_dst(), 
                   p->get_angle(),
                   NULL, SDL_FLIP_NONE);

  if (ret < 0)
    ret_err_SDL("Renderer copy! SDL_Error");
}

inline void SDL_wrapper::rend_clear(const std::shared_ptr<SDL_Renderer> rend)
{
  if (SDL_RenderClear(rend.get()) < 0)
    ret_err_SDL("Renderer clear! SDL_Error");
}