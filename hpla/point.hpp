#pragma once

#include <cmath>
#include <string>


template<class T = long double>
struct point {
  using value_type = T;

  point(value_type x = 0,
        value_type y = 0,
        value_type z = 0)
    : x(x)
    , y(y)
    , z(z)
  {}

  point operator+(const point& p) const {
    return point(x + p.x, y + p.y, z + p.z);
  }

  point operator+(value_type v) const {
    return point(x + v, y + v, z + v);
  }

  point operator-(const point& p) const {
    return point(x - p.x, y - p.y, z - p.z);
  }

  point operator/(value_type p) const {
    return point(x / p, y / p, z / p);
  }

  point operator*(value_type p) const {
    return point(x * p, y * p, z * p);
  }

  point pow(int a) const {
    return point(std::pow(x, a), std::pow(y, a), std::pow(z, a));
  }

  long double length() const {
    return std::sqrt(x*x + y*y + z*z);
  }

  point direction() const {
    return (*this) / this->length();
  }

  point inv() const {
    return (*this) * -1;
  }

  std::string tostring() const {
    return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
  }

  template<class Tp>
    point<Tp> astype() {
      return point<Tp>(x, y, z);
    }

  value_type x, y, z;
};


template<class T>
  std::ostream& operator<<(std::ostream& os, const point<T>& p)
  {
    return os << p.x << ", " << p.y << ", " << p.z;
  }
