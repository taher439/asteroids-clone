#include "sdl_wrapper.h"
#include "player.h"

void 
SDL_wrapper::ret_err_SDL(const std::string& err_msg) 
{
  std::cerr << err_msg << ": " << SDL_GetError() << std::endl;
  exit(EXIT_FAILURE);
}

void
SDL_wrapper::init(void) 
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
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

std::shared_ptr<SDL_Texture>    
SDL_wrapper::create_texture(const std::shared_ptr<SDL_Renderer>& rend, int height, int width) 
{
  std::shared_ptr<SDL_Texture> texture = 
    sdl_shared(SDL_CreateTexture(rend.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height));
  
  if (texture == nullptr) 
    ret_err_SDL("Texture could not be created! SDL_Error");
  return texture;
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

SDL_Surface* 
SDL_wrapper::load_surface(char const *path)
{
    SDL_Surface *image_surface = SDL_LoadBMP(path);

    if(!image_surface)
        return 0;

    return image_surface;
}

std::shared_ptr<SDL_Texture>
SDL_wrapper::load_texture(const std::shared_ptr<SDL_Renderer>& rend, std::string const path)
{
    SDL_Surface * image = load_surface(path);
    if (image == 0) { 
      SDL_wrapper::ret_err_SDL("Couldn't find image");
    }
    // set black as a transparent color
    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));

    std::shared_ptr<SDL_Texture> texture = 
    sdl_shared(SDL_CreateTextureFromSurface(rend.get(), image));
  
    SDL_FreeSurface(image);
    
    if (texture == nullptr) 
        ret_err_SDL("Texture could not be created! SDL_Error");
    return texture;
}

// https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl
void 
SDL_wrapper::draw_circle(const std::shared_ptr<SDL_Renderer>& rend, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(rend.get(), centreX + x, centreY - y);
      SDL_RenderDrawPoint(rend.get(), centreX + x, centreY + y);
      SDL_RenderDrawPoint(rend.get(), centreX - x, centreY - y);
      SDL_RenderDrawPoint(rend.get(), centreX - x, centreY + y);
      SDL_RenderDrawPoint(rend.get(), centreX + y, centreY - x);
      SDL_RenderDrawPoint(rend.get(), centreX + y, centreY + x);
      SDL_RenderDrawPoint(rend.get(), centreX - y, centreY - x);
      SDL_RenderDrawPoint(rend.get(), centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

std::map<char, std::shared_ptr<SDL_Texture>>
SDL_wrapper::numbers {
  {'0', nullptr},
  {'1', nullptr},
  {'2', nullptr},
  {'3', nullptr},
  {'4', nullptr},
  {'5', nullptr},
  {'6', nullptr},
  {'7', nullptr},
  {'8', nullptr},
  {'9', nullptr}
};

std::shared_ptr<SDL_Texture> SDL_wrapper::life = nullptr;
std::shared_ptr<SDL_Texture> SDL_wrapper::dead = nullptr;

void
SDL_wrapper::load_textures(const std::shared_ptr<SDL_Renderer>& rend)
{
  SDL_wrapper::numbers['0'] = SDL_wrapper::load_texture(rend, DATA_PATH+"zero.bmp");
  SDL_wrapper::numbers['1'] = SDL_wrapper::load_texture(rend, DATA_PATH+"one.bmp");
  SDL_wrapper::numbers['2'] = SDL_wrapper::load_texture(rend, DATA_PATH+"two.bmp");
  SDL_wrapper::numbers['3'] = SDL_wrapper::load_texture(rend, DATA_PATH+"three.bmp");
  SDL_wrapper::numbers['4'] = SDL_wrapper::load_texture(rend, DATA_PATH+"four.bmp");
  SDL_wrapper::numbers['5'] = SDL_wrapper::load_texture(rend, DATA_PATH+"five.bmp");
  SDL_wrapper::numbers['6'] = SDL_wrapper::load_texture(rend, DATA_PATH+"six.bmp");
  SDL_wrapper::numbers['7'] = SDL_wrapper::load_texture(rend, DATA_PATH+"seven.bmp");
  SDL_wrapper::numbers['8'] = SDL_wrapper::load_texture(rend, DATA_PATH+"height.bmp");
  SDL_wrapper::numbers['9'] = SDL_wrapper::load_texture(rend, DATA_PATH+"nine.bmp");
  SDL_wrapper::life = SDL_wrapper::load_texture(rend, DATA_PATH+"heart.bmp");
  SDL_wrapper::dead = SDL_wrapper::load_texture(rend, DATA_PATH+"dead.bmp");
}

int
SDL_wrapper::update_score(
  const std::shared_ptr<SDL_Renderer>& rend, 
  std::shared_ptr<SDL_Texture> texture,
  const int score)
{
  int height, width;
  height = NUMBER_HEIGHT; width = MAX_DIGITS*NUMBER_WIDTH;

  std::string string_score = std::to_string(score);

  // draw on the texture
  SDL_SetRenderTarget(rend.get(), texture.get());
  SDL_RenderClear(rend.get());

  // digit position on the texture
  SDL_Rect drect;
  drect.x = 0; drect.y = 0; drect.h = NUMBER_HEIGHT; drect.w = NUMBER_WIDTH;

  for (auto n: string_score) {
    SDL_RenderCopy(rend.get(), SDL_wrapper::numbers[n].get(), NULL, &drect);
    drect.x += NUMBER_WIDTH;
  }

  // reset renderer
  SDL_SetRenderTarget(rend.get(), NULL);

  // return the number width on the texture
  return string_score.length()*NUMBER_WIDTH;
}

int
SDL_wrapper::update_life(
  const std::shared_ptr<SDL_Renderer>& rend, 
  std::shared_ptr<SDL_Texture> texture,
  const int life)
{
  int height, width, ret_width;
  height = LIFE_HEIGHT; width = MAX_LIFE*LIFE_WIDTH;

  // draw on the texture
  SDL_SetRenderTarget(rend.get(), texture.get());
  SDL_RenderClear(rend.get());

  // heart position on the texture
  SDL_Rect drect;
  drect.x = 0; drect.y = 0; drect.h = LIFE_HEIGHT; drect.w = LIFE_WIDTH;

  if (life <= 0 ) {
    std::cout << "Here" << std::endl;
    SDL_RenderCopy(rend.get(), SDL_wrapper::dead.get(), NULL, &drect);
    ret_width = LIFE_WIDTH;
  }
  else {
    for (int i = 0; i < life; i++) {
      SDL_RenderCopy(rend.get(), SDL_wrapper::life.get(), NULL, &drect);
      drect.x += LIFE_WIDTH;
    }
    ret_width = life*LIFE_WIDTH;
  }

  // reset renderer
  SDL_SetRenderTarget(rend.get(), NULL);

  // return the src width on the texture
  return ret_width;
}