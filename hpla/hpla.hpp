#pragma once

#include "signal.hpp"

using ld = long double;


inline constexpr ld CLIGHT = 299792458.0;


std::pair<ldpoint, ldpoint> hpla(signal i, signal j,
                                 signal k, signal l);

ld eps(const signal& a, const signal& b, const ldpoint& p);
