#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <sstream>

// Intel performance counters
#include "cpucounters.h"

typedef std::vector<double> Points_t;

template <class T>
void doNotOptimizeAway(T&& datum) {
  asm volatile("" : "+r" (datum));
}

#define M 2*52428800 // about 100MB
#define N 200
Points_t* prepare() {
  std::mt19937_64 generator;
  generator.seed(123456.);
  std::normal_distribution<double> normal_dist(5.0, 4.0);
  auto normal = std::bind(normal_dist, generator);
  
  auto data = new Points_t;
  data->reserve(N);

  // somewhere to read and write to fill
  // the cahces with irrelevant stuff
  std::vector<char> thrash;
  thrash.reserve(M);

  for (auto p = 0; p < N; ++p) {
    data->push_back(normal());
  }

  for (auto i=0; i<M; ++i) {
    thrash.push_back(i*3);
  }

  // these are only here to serve as a way of stopping
  // the optimiser from getting rid of the loop bodies
  volatile auto blah(0);
  for (auto i=0; i<M; ++i) {
    blah += thrash[i];
  }
  doNotOptimizeAway(blah);
  
  return data;
}

// Binary search
Points_t::iterator
std_binary(Points_t* data, double X) {
  auto first = std::lower_bound(data->begin(), data->end(), X,
				[](const double& a, const double& x)->bool{
				  return a < x;
				});
  doNotOptimizeAway(first);
  return first;
}

// Simply use find!
Points_t::iterator
std_find(Points_t* data, double X) {
  auto first = std::find_if(data->begin(), data->end(),
			    [&X](const double& a)->bool{
                              return a >= X;
                            }
			   );
  doNotOptimizeAway(first);
  return first;
}

int main(int argc, char *argv[]) {
  PCM * m = PCM::getInstance();
  m->program(PCM::DEFAULT_EVENTS, NULL);

  if (m->program() != PCM::Success) {
    std::cout << "PMU not available, re-running will probably fix this" << std::endl;
    return 1;
  }

  if (argc < 3) {
    std::cout << "You need to provide the method and desired search value as arguments"
	      << std::endl;
    return 1;
  }
  std::string method(argv[1]);
  std::istringstream iss(argv[2]);
  double X;
  iss >> X;

  auto d = prepare();
  std::cout <<"min value: " << *std::min_element(d->begin(), d->end()) <<" max value: ";
  std::cout << *std::max_element(d->begin(), d->end()) << std::endl;

  auto impl = [](Points_t* d, double x){return std_binary(d, x);};
  if (method == "std_bin") {
    auto impl = [](Points_t* d, double x){return std_binary(d, x);};
  }
  else if (method == "std_find") {
    auto impl = [](Points_t* d, double x){return std_find(d, x);};
  }
  else {
    std::cout << "Not a valid method name." << std::endl;
    return 1;
  }
  
  SystemCounterState before = getSystemCounterState();

  auto s=impl(d, X);

  SystemCounterState after = getSystemCounterState();
  std::cout << "CPU cycles used: " << getRefCycles(before, after) <<
    "\nInstructions: " << getInstructionsRetired(before, after) <<
    "\nInstructions per Clock: " << getIPC(before, after) <<
    "\nL3 cache hit ratio: " << getL3CacheHitRatio(before, after) <<
    "\nL2 cache hit ratio: " << getL2CacheHitRatio(before, after) <<
    "\nWasted cycles caused by L3 misses: " << getCyclesLostDueL3CacheMisses(before, after) <<
    "\nBytes read from DRAM: " << getBytesReadFromMC(before, after) <<
    std::endl;
  
  m->cleanup();
  return 0;
}
