#include <iomanip>
#include <random>
#include <limits>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <numbers>
#include <array>
#include <fstream>
#include <list>
#include <vector>

#include "hpla.hpp"
#include "point.hpp"
#include "signal.hpp"


std::pair<ldpoint, ldpoint> hpla(signal i, signal j, signal k, signal l)
{
  ldpoint offset = l.location;

  i.location = i.location - offset;
  j.location = j.location - offset;
  k.location = k.location - offset;
  l.location = l.location - offset;

  ld rik = (i.time - k.time) * CLIGHT;
  ld rij = (i.time - j.time) * CLIGHT;
  ld rkj = (k.time - j.time) * CLIGHT;
  ld rkl = (k.time - l.time) * CLIGHT;

  auto [xi, yi, zi] = i.location;
  auto [xi2, yi2, zi2] = i.location.pow(2);
  auto [xj2, yj2, zj2] = j.location.pow(2);
  auto [xk2, yk2, zk2] = k.location.pow(2);
  auto [xl2, yl2, zl2] = l.location.pow(2);

  auto [xji, yji, zji] = j.location - i.location;
  auto [xki, yki, zki] = k.location - i.location;
  auto [xjk, yjk, zjk] = j.location - k.location;
  auto [xlk, ylk, zlk] = l.location - k.location;

  ld A = (rik*xji - rij*xki) / (rij*yki - rik*yji);
  ld B = (rik*zji - rij*zki) / (rij*yki - rik*yji);

  ld c0 = rik * (rij*rij + (xi2 - xj2 + yi2 - yj2 + zi2 - zj2));
  ld c1 = rij * (rik*rik + (xi2 - xk2 + yi2 - yk2 + zi2 - zk2));
  ld c2 = 2 * (rij*yki - rik*yji);

  ld C = (c0 - c1) / c2;

  ld D = (rkl*xjk - rkj*xlk) / (rkj*ylk - rkl*yjk);
  ld E = (rkl*zjk - rkj*zlk) / (rkj*ylk - rkl*yjk);

  ld f0 = rkl * (rkj*rkj + xk2 - xj2 + yk2 - yj2 + zk2 - zj2);
  ld f1 = rkj * (rkl*rkl + xk2 - xl2 + yk2 - yl2 + zk2 - zl2);
  ld f2 = 2 * (rkj*ylk - rkl*yjk);

  ld F = (f0 - f1) / f2;

  ld G = (E-B) / (A-D);
  ld H = (F-C) / (A-D);

  ld I = A*G + B;
  ld J = A*H + C;
  ld K = rik*rik + xi2 - xk2 + yi2 - yk2 + zi2 - zk2 + 2*xki*H + 2*yki*J;
  ld L = 2 * (xki*G + yki*I + 2*zki);

  ld M = 4 * rik*rik * (G*G + I*I + 1) - L*L;

  ld n0 = G*(xi-H);
  ld n1 = I*(yi-J);
  ld n2 = 8 * (rik*rik);
  ld N = n2 * (n0 + n1 + zi) + 2*L*K;

  ld o0 = (xi - H)*(xi - H);
  ld o1 = (yi - J)*(yi - J);
  ld o2 = 4 * (rik*rik);
  ld O = o2 * (o0 + o1 + zi2) - K*K;

  ld P = N / (2*M);
  ld root = std::sqrt(P*P - O/M);

  ld z0 = (P + root);
  ld x0 = G*z0 + H;
  ld y0 = I*z0 + J;

  ld z1 = (P - root);
  ld x1 = G*z1 + H;
  ld y1 = I*z1 + J;

  return { ldpoint{x0, y0, z0} + offset, ldpoint{x1, y1, z1} + offset };
}


ld eps(const signal& a, const signal& b, const ldpoint& p)
{
  ld dradius = std::abs(a.time - b.time) * CLIGHT;
  ld diff = std::abs((a.location - p).length() - (b.location - p).length()) - dradius;
  return diff*diff;
}
