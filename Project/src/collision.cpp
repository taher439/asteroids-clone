#include "collision.h"

bool 
collision::point_in_ellipse(const Vec2<double>& p, const Vec2<double>& e_center, double rx, double ry)
{
  double dx, dy, dist; 

  // square ellipse radius
  double rx2 = rx * rx;
  double ry2 = ry * ry;
  auto ellipse = [&](double x, double y) { return ry2*x*x + rx2*y*y - rx2*ry2;};

  dx = abs(e_center.x - p.x);
  dy = abs(e_center.y - p.y);
  dist = ellipse(dx, dy);
  return dist <= 0;
}

bool
collision::point_in_triangle(const Vec2<double>& p, const Vec2<double>& t1, const Vec2<double>& t2, const Vec2<double>& t3)
{
  // http://www.jeffreythompson.org/collision-detection/tri-point.php
  auto doubled_triangle_area = 
    [&](double x1, double y1, double x2, double y2, double x3, double y3) 
    { 
      return abs( (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1) );
    };

  // all the triangle areas are doubled, the division by 2 isn't necessary for the comparison
  double original_area = doubled_triangle_area(t1.x, t1.y, t2.x, t2.y, t3.x, t3.y);

  double area1 = doubled_triangle_area(p.x, p.y, t2.x, t2.y, t3.x, t3.y);
  double area2 = doubled_triangle_area(t1.x, t1.y, p.x, p.y, t3.x, t3.y);
  double area3 = doubled_triangle_area(t1.x, t1.y, t2.x, t2.y, p.x, p.y);

  return area1 + area2 + area3 - original_area <= EPSILON;
}


double 
collision::squared_dist(const Vec2<double>& a, const Vec2<double>& b)
{
  return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}

// detect collision between segment from l_a to l_b with the circle of center c_center and radius c_radius
bool 
collision::line_circle(const Vec2<double>& l_a, const Vec2<double>& l_b, const Vec2<double>& c_center, double c_radius)
{
  // http://yehar.com/blog/?p=2926
  double squared_radius = c_radius * c_radius;

  // translate everything so that l_a is the origin
  Vec2<double> ab = l_b - l_a;       // new segment (vector of origin (0,0))
  Vec2<double> cd = c_center - l_a;  // new circle center

  // if one of the line extremity is in the circle
  if (cd.x*cd.x + cd.y *cd.y <= squared_radius || collision::squared_dist(ab, cd) <= squared_radius )
    return true; 

  // scaling and rotating everything so that the ligne segment is horizontal
  Vec2<double> new_c_center (cd.x*ab.x + cd.y*ab.y, cd.y*ab.x - cd.x*ab.y);

  ab.x = ab.x*ab.x + ab.y*ab.y;
  ab.y = 0; 

  // there is an intersection if :
  return (new_c_center.y*new_c_center.y <= squared_radius * ab.x  // the circle crosses the horizontal line
      &&  new_c_center.x >= ab.x     // and the circle center is "over" or "under"
      &&  new_c_center.x <= ab.y     // the scaled and rotated segment 
  );
}
// testing
// if (collision::line_circle(Vec2<double>(1.1, 13.2), Vec2<double>(5.7, 20.9), Vec2<double>(6.35,16.33), 3))
// {
//   std::cout << "collision" << std::endl;
// } else {
//   std::cout << "no collision" << std::endl;
// }