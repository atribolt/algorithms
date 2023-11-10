#include <list>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "hpla.hpp"
#include "index_block.hpp"


ldpoint correction(const signal& i, const signal& j, ldpoint mean)
{
  ldpoint dir = mean.direction();

  ld rij = (i.time - j.time) * CLIGHT;

  ld id = (i.location - mean).length();
  ld jd = (j.location - mean).length();

  ld d = rij - (id - jd);
  return mean + dir * d;
}


ldpoint correction(const signal& i, const signal& j, const signal& k, const signal& l, ldpoint mean)
{
  ldpoint ik = correction(i, k, mean);
  ldpoint il = correction(i, l, mean);
  ldpoint kj = correction(k, j, mean);
  ldpoint kl = correction(k, l, mean);

  return (ik + il + kj + kl) / 4;
}


ldpoint tdoa(std::vector<signal> s)
{
  std::sort(s.begin(), s.end(), std::greater<signal>());

  constexpr int step = 3;

  ld min_eps = std::numeric_limits<ld>::max();
  ldpoint mean;

  for (int i = 1; (i + step) < s.size(); ++i) {
    signal si = s[i];
    signal sj = s[i+1];
    signal sk = s[i+2];
    signal sl = s[i+3];

    auto [p0, p1] = hpla(si, sj, sk, sl);

    ld e0 = eps(s[0], s[i+2], p0);
    ld e1 = eps(s[0], s[i+2], p1);

    if (e0 < e1)
    {
      if (e0 < min_eps)
      {
        mean = p0;
        min_eps = e0;
      }
    }
    else
    {
      if (e1 < min_eps)
      {
        mean = p1;
        min_eps = e1;
      }
    }
  }

  for (int i = 0; i < 1000; ++i)
    mean = correction(s[0], s[1], s[2], s[3], mean);

  return mean;
}


bool test_function(std::vector<ldpoint> detectors, ldpoint sig)
{
  std::vector<signal> s;

  for (int i = 0; i < detectors.size(); ++i)
  {
    s.push_back({});
    s[i].location = detectors[i];
    s[i].time = (sig - detectors[i]).length() / CLIGHT;
  }

  ldpoint p = tdoa(s);

  return ((sig - p).length() < 1);
}


ldpoint test_function1(std::vector<ldpoint> detectors, ldpoint sig)
{
  std::vector<signal> s;

  for (int i = 0; i < detectors.size(); ++i)
  {
    s.push_back({});
    s[i].location = detectors[i];
    s[i].time = (sig - detectors[i]).length() / CLIGHT;
  }

  return tdoa(s);
}


std::list<ldpoint> test_block(std::vector<ldpoint> detectors, ldpoint beg, ldpoint end)
{
  std::list<ldpoint> result;
  index_block block(beg.astype<ssize_t>(), end.astype<ssize_t>());

  std::cout << "Total points: " << block.count() << std::endl;

  auto idx = block.begin();
  while (idx != block.end()) {
    ldpoint sig = (*idx).astype<ld>();

    if (test_function(detectors, sig))
      result.push_back(sig);

    ++idx;
  }

  return result;
}

std::list<ldpoint> test_block1(std::vector<ldpoint> detectors, ldpoint beg, ldpoint end)
{
  std::list<ldpoint> result;
  index_block block(beg.astype<ssize_t>(), end.astype<ssize_t>());

  std::cout << "Total points: " << block.count() << std::endl;

  auto idx = block.begin();
  while (idx != block.end()) {
    ldpoint sig = (*idx).astype<ld>();

    auto p = test_function1(detectors, sig);
    result.push_back(sig);
    result.push_back(p);

    ++idx;
  }

  return result;
}


int main()
{
  std::vector<ldpoint> detectors = {
    ldpoint(3094630.38854713, 2505425.09092982, 4966104.15216029),
    ldpoint(3079941.17945729, 2485614.63623443, 4984988.94806104),
    ldpoint(3062635.24265019, 2494808.50975087, 4991077.86427372),
    ldpoint(3074042.88562686, 2498557.10583058, 4982228.62403244),
    ldpoint(3069056.55181741, 2502835.25511881, 4983149.11308065),
    ldpoint(3071059.44092061, 2508667.55550352, 4979008.58710876),
    ldpoint(3075313.97383195, 2504676.64226779, 4978395.87629038)
  };

  ldpoint begin = ldpoint{ 3000000, 2500000, 5000000 };
  auto result = test_block1(detectors, begin, begin + 100);

  std::cout << "computed: " << result.size() << std::endl;

  std::ofstream out { "dots.dump" };

  for (auto& p : result) {
    out << std::setprecision(24) << p.x << " " << p.y << " " << p.z << "\n";
  }

  out.close();
  return 0;
}
