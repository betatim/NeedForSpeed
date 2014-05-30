// (C) 2014 Tim Head
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <stdint.h>

// Compare using std::lower_bound and a home made binary
// search to find first 'Hit' with x above some value
// also compares usinga Hit object and a vector of doubles
// containing just the x coordinate.

#ifdef __i386
__inline__ uint64_t rdtsc() {
  uint64_t x;
  __asm__ volatile("rdtsc" : "=A"(x));
  return x;
}
#elif __amd64
__inline__ uint64_t rdtsc() {
  uint64_t a, d;
  __asm__ volatile("rdtsc" : "=a"(a), "=d"(d));
  return (d << 32) | a;
}
#endif
class Timer {
public:
  Timer() : _start(rdtsc()) {}
  void reset() { _start = rdtsc(); }
  void stop() { _stop = rdtsc(); }
  uint64_t delta() { return _stop - _start; }

private:
  uint64_t _start;
  uint64_t _stop;
};

// when building tracks in a particle physics
// experiment you need to have at least the 3D space
// point and usually some extra information
class Hit {
public:
  Hit(double x, double y, double z) : x(x), y(y), z(z) {}

  double x;
  double y;
  double z;
  //long long id;
};

// Standard binary search
std::vector<Hit>::iterator
std_algorithm(std::vector<Hit>& hits, double desired) {
  auto first = std::lower_bound(hits.begin(), hits.end(), desired,
				[](const Hit& a, const double& x) {
				  return a.x < x;
				});
  return first;
}

// Simply use find!
std::vector<Hit>::iterator
std_find(std::vector<Hit>& hits, double desired) {
  auto first = std::find_if(hits.begin(), hits.end(),
			    [&desired](const Hit& a) {
                              return a.x >= desired;
                            }
			   );
  return first;
}

// Home made binary search
unsigned int
self_made_binary(std::vector<Hit>& hits, double desired) {
    unsigned int idx(0);
    unsigned int step(hits.size());

    while (2 < step) {
      step /= 2;
      if (hits[idx + step].x < desired)
	idx += step;
    }
    // might end up below where we want to be
    // so we just walk up
    while (hits[idx].x < desired) {
      idx++;
    }
    return idx;
}

int main(void) {
  // randomly generate some hits with a mean of 5 and spread of 4
  std::mt19937_64 generator;
  generator.seed(123456.);
  std::normal_distribution<double> normal_dist(5.0, 4.0);
  auto normal = std::bind(normal_dist, generator);
  
  int n_loops(20000);
  int n_points(400);
  Timer t;
  uint64_t time(0);
  std::vector<uint64_t> times;
  times.reserve(n_loops);
  
  // You might expect that std::find would win
  // if the value you are looking for is close
  // to the beginning, so try different values here
  double desired;
  desired = -1.1;
  desired = 5.;
  desired = 3.;

  std::vector<double> points;
  points.reserve(n_points);
  std::vector<Hit> hits;
  hits.reserve(n_points);

  for (auto loop = 0; loop < n_loops; ++loop) {
    points.clear(); hits.clear();
    for (auto p = 0; p < n_points; ++p) {
      auto x = normal();
      points.push_back(x);
      hits.push_back(Hit(x, normal(), normal()));
    }

    t.reset();
    // sort points and hits by x
    std::sort(hits.begin(), hits.end(),
	      [](const Hit& a, const Hit& b)->bool {
		return a.x < b.x;
	      });
    std::sort(points.begin(), points.end());
    
#if STD
    // std lib binary search
    auto itr(std_algorithm(hits, desired));
#endif

#if FIND
    // std find
    auto itr(std_find(hits, desired));
#endif

#if SELF
    // home made binary search
    auto itr(self_made_binary(hits, desired));
#endif

    t.stop();
    times.push_back(t.delta());
  }
  
  uint64_t total(std::accumulate(times.begin(), times.end(), 0.));
  uint64_t min(*std::min_element(times.begin(), times.end()));
  std::cout << "total: " << total
	    << " avg per loop: " << total / n_loops
	    << " minimum: " << min << std::endl;
}
