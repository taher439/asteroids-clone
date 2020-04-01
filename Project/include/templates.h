#ifndef _TEMPLATES_H
#define _TEMPLATES_H

#include <memory>
#include <random>
#include <vector>
#include <type_traits>

class Particle;

template <typename T>
class Vec2 {
  static_assert(std::is_arithmetic_v<T>);
  public:
    T x, y;
    Vec2(): x(0), y(0) {}
    Vec2(const T& _x, const T& _y): x(_x), y(_y) {}
    Vec2(const Vec2<T>& v): x(v.x), y(v.y) {}
    inline Vec2<T> operator+(const Vec2& v) const {
      return Vec2(this->x + v.x, this->y + v.y);
    }

    inline Vec2<T> operator-(const Vec2& v) const {
      return Vec2(this->x - v.x, this->y - v.y);
    }

    inline Vec2<T> operator*(const T& val) const {
      return Vec2(this->x * val, this->y * val);
    }

    inline Vec2<T> operator+(const T& val) const {
      return Vec2(this->x + val, this->y + val);
    }

    inline Vec2<T> operator-(const T& val) const {
      return Vec2(this->x - val, this->y - val);
    }

    inline Vec2<T>& operator += (const Vec2<T>& v) {
      x += v.x;
      y += v.y;
      return *this;
    }

    inline Vec2<T>& operator += (const T& val) {
      x += val;
      y += val;
      return *this;
    }

    inline Vec2<T>& operator -= (const Vec2<T>& v){
      x -= v.x;
      y -= v.y;
      return *this;
    }

    inline Vec2<T>& operator *= (const T& s){
    x *= s;
    y *= s;
    return *this;
    }
  
    inline Vec2<T>& operator *= (const Vec2<T>& v){
      x *= v.x;
      y *= v.y;
      return *this;
    }

    inline Vec2<T>& operator /= (const T& s){
      x /= s;
      y /= s;
      return *this;
    }

    inline friend std::ostream& operator << (std::ostream &out, const Vec2<T>& v){
      out << v.x << " | " << v.y;
      return out;
    }
  
    void print() {
#ifdef DEBUG
      std::cout << this->x << " " << this->y << std::endl;
#endif
    }
};

template <typename T, typename = void>
class Rand_gen;

template <typename T>
class Rand_gen <T, std::enable_if_t<std::is_floating_point_v<T>>>
{
  public:
    static T rand_num(const T& min, const T& max) {
      std::random_device rd; 
      std::mt19937 gen(rd()); 
      std::uniform_real_distribution<T> dis(min, max);
      return dis(gen);
    }

    static T rand_gauss(const T& min, const T& max) {
      std::random_device rd; 
      std::mt19937 gen(rd()); 
      std::normal_distribution<T> dis(min, max);
      return dis(gen);
    }
};

template <typename T>
class Rand_gen <T, std::enable_if_t<std::is_integral_v<T>>>
{
  public:
    static T rand_num(const T& min, const T& max) {
      std::random_device rd; 
      std::mt19937 gen(rd()); 
      std::uniform_int_distribution<T> dis(min, max);
      return dis(gen);
    }
  
    static T rand_gauss(const T& min, const T& max) {
      std::random_device rd; 
      std::mt19937 gen(rd()); 
      std::normal_distribution<T> dis(min, max);
      return dis(gen);
    }
};

#endif