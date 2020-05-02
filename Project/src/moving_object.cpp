#include "moving_object.h"
#include "sdl_wrapper.h"

Moving_object::Moving_object (void)
{
  this->direction = Direction::normed();
  this->dstrect = {0, 0, 0, 0};
  this->speed = 1; // TODO: modify speed
}

Moving_object::Moving_object (const std::shared_ptr<SDL_Renderer>& rend, int height, int width)
{
  this->rend = rend;
  this->center = this->generate_center_pos();
  this->top_left.x = this->center.x - width/2;
  this->top_left.y = this->center.y - height/2;
  this->dstrect.x = (int) this->top_left.x;
  this->dstrect.y = (int) this->top_left.y;
  this->dstrect.h = height;
  this->dstrect.w = width;
  this->direction = Direction::normed();
  this->height = height;
  this->width = width;
  this->speed = 1; // TODO: modify speed
}


void 
Moving_object::draw () 
{
//   std::cout << this->top_left.x << "|" << this->top_left.y << std::endl;
  this->dstrect.x = (int) this->top_left.x;
  this->dstrect.y = (int) this->top_left.y;
  SDL_RenderCopy(this->rend.get(), this->texture.get(),  NULL,  &(this->dstrect));
}

void 
Moving_object::set_position (double x, double y) 
{
  this->center.x = x;
  this->center.y = y;
  this->top_left.x = this->center.x - this->width/2;
  this->top_left.y = this->center.y - this->height/2;
}

void 
Moving_object::set_size (int height, int width) 
{
  this->dstrect.h = height;
  this->dstrect.w = width;
}

void 
Moving_object::set_speed (double s) 
{
  this->speed = s;
}

void 
Moving_object::move (void)
{
  this->center += this->direction * this->speed;
  this->top_left += this->direction * this->speed;
  if (!in_screen(this->center)) {
    this->wrap_coord(this->center);
    this->top_left.x = this->center.x - this->width/2;
    this->top_left.y = this->center.y - this->height/2;
  }  
}

void 
Moving_object::wrap_coord (Vec2<double>& v)
{ // TODO: use globals for screen size
  if (v.x > 640) 
    v.x = 0;
  if (v.x < 0) 
    v.x = 640;
  
  if (v.y > 480) 
    v.y = 0;
  if (v.y < 0) 
    v.y = 480;
}

bool 
Moving_object::in_screen (const Vec2<double>& v)
{ // TODO: use globals for screen size
  bool in_flag = true;
  if (v.x > 640) {
    in_flag = false;
  }
  if (v.x < 0) {
    in_flag = false;
  }
  
  if (v.y > 480) {
    in_flag = false;
  }
  if (v.y < 0) {
    in_flag = false;
  }
  return in_flag;
}

Vec2<double>
Moving_object::generate_center_pos(void)
{
  Vec2<double> center;
  
  // make asteroids spawn
  center.x = Rand_gen<double>::rand_num(0, 640);
  center.y = Rand_gen<double>::rand_num(0, 480);
  // std::cout << "center at pos" << center << std::endl;

  // to select a coordinate
  int c = Rand_gen<int>::rand_num(0,1);
  // to select min or max
  int m = Rand_gen<int>::rand_num(0,1);
  if (c == 0) {
    // x coordinate is chosen
    if (m == 0) {
      // set to minimal value
      center.x = 0;
    } else {
      // set to maximal value
      center.x = 640;
    }
  } else {
    // y coordinate is chosen
    if (m == 0) {
      // set to minimal value
      center.y = 0;
    } else {
      // set to maximal value
      center.y = 480;
    }
  }

  return center;
}


////////////////////////////////////////////////////////////////////////////////

// Moving_object_container::Moving_object_container ()
// {
//   this->total_objects = 0;
// }

// void
// Moving_object_container::add_moving_object(std::shared_ptr<Moving_object> m)
// {
//   this->total_objects++;
//   this->moving_objects.emplace_back(m);
// }

// void 
// Moving_object_container::update_all (std::vector<std::shared_ptr<blast>>& blasts)
// {
//   for (auto m = this->moving_objects.begin(); m != this->moving_objects.end(); m++) {
//     (*m)->update();
//     (*m)->detect_collision(blasts);
//   }
// }

// void 
// Moving_object_container::draw_all (void)
// {
//   for (auto m = this->moving_objects.begin(); m != this->moving_objects.end(); m++) {
//     (*m)->draw();
//   }
// }

// void 
// Moving_object_container::clean_all (void)
// {
//   for (auto m = this->moving_objects.begin(); m != this->moving_objects.end(); m++) {
//     if ( !(*m)->is_alive() ) {
//       m = this->moving_objects.erase(m);
//       m--;
//       this->total_objects--;
//     }
//   }
// }

// bool
// Moving_object_container::is_empty (void)
// {
//   return this->total_objects == 0;
// }