#ifndef _COLLISION_H
#define _COLLISION_H

#include <iostream>
#include "templates.h"
#define EPSILON 0.5


namespace collision
{
  bool    point_in_ellipse    (const Vec2<double>& p, const Vec2<double>& e_center, double rx, double ry);
  bool    point_in_triangle   (const Vec2<double>& p, const Vec2<double>& t1, const Vec2<double>& t2, const Vec2<double>& t3);
  bool    line_circle         (const Vec2<double>& l_a, const Vec2<double>& l_b, const Vec2<double>& c_center, double c_radius);

  double  squared_dist        (const Vec2<double>& a, const Vec2<double>& b);
}

#endif