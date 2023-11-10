#pragma once

#include "point.hpp"


using ldpoint = point<long double>;


struct signal {
  ldpoint location {};
  long double time {0.0};
};


inline bool operator<(const signal& self, const signal& o) {
  return self.time < o.time;
}


inline bool operator>(const signal& self, const signal& o) {
  return self.time > o.time;
}
