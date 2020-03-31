#include "sdl_wrapper.h"
#include "player.h"

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
SDL_wrapper::creat_rend(const std::shared_ptr<SDL_Window>& win) 
{
  std::shared_ptr<SDL_Renderer> rend = 
    sdl_shared(SDL_CreateRenderer(win.get(), 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
  
  if (rend == nullptr) 
    ret_err_SDL("Renderer could not be created! SDL_Error");
  return rend;
}

void 
SDL_wrapper::rend_copy_ex(const std::shared_ptr<SDL_Renderer>& rend, 
                          const std::shared_ptr<Player>& p) 
{
  int ret;
  ret = SDL_RenderCopyEx(rend.get(), 
                   p->get_tex().get(), 
                   p->get_rect_src().get(), 
                   p->get_rect_dst().get(), 
                   p->get_angle(),
                   NULL, SDL_FLIP_NONE);

  if (ret < 0)
    ret_err_SDL("Renderer copy! SDL_Error");
}

void 
SDL_wrapper::rend_clear(const std::shared_ptr<SDL_Renderer>& rend)
{
  if (SDL_RenderClear(rend.get()) < 0)
    ret_err_SDL("Renderer clear! SDL_Error");
}

void
SDL_wrapper::draw_line(const std::shared_ptr<SDL_Renderer>& rend,
        const  int&   x1,
        const  int&   y1,
        const  int&   x2,
        const  int&   y2)
{
  SDL_SetRenderDrawColor(rend.get(), 255, 255, 255, 255);
  SDL_RenderDrawLine(rend.get(), x1, y1, x2, y2);
}

void
SDL_wrapper::fill_screen(const std::shared_ptr<SDL_Window>& win) 
{
  SDL_Surface *surface = SDL_GetWindowSurface(win.get());
  Uint32 skyblue=SDL_MapRGB(surface->format, 65,193,193);
  SDL_FillRect(surface, NULL, skyblue);
  SDL_UpdateWindowSurface(win.get());
}

void 
SDL_wrapper::draw_point(const std::shared_ptr<SDL_Renderer>& rend, 
                        const int& x, 
                        const int& y)
{
  int ret = SDL_RenderDrawPoint(rend.get(), x, y);
  if (ret < 0)
    ret_err_SDL("SDL drax point Error");
}