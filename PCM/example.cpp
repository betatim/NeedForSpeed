#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>

// Intel performance counters
#include "cpucounters.h"

template <class T>
void doNotOptimizeAway(T&& datum) {
  asm volatile("" : "+r" (datum));
}

#define N 2*52428800 // about 100MB
std::vector<char>* prepare() {
  auto data = new std::vector<char>;
  data->reserve(N);

  char letters[] = "abcdefghijklmnopqrstuvwxyz";

  // somewhere to read and write to fill
  // the caches with irrelevant stuff
  std::vector<char> thrash;
  thrash.reserve(N);

  for (auto i=0; i<N; ++i) {
    data->push_back(letters[i%26]);
  }

  for (auto i=0; i<N; ++i) {
    thrash.push_back(i*3);
  }

  // these are only here to serve as a way of stopping
  // the optimiser from getting rid of the loop bodies
  auto blah(0);
  for (auto i=0; i<N; ++i) {
    blah += thrash[i];
  }
  doNotOptimizeAway(blah);
  return data;
}

int test(std::vector<char>* data, unsigned int step, unsigned int reads) {
  int sum(0);
  auto s(0);
  for (auto i=0; i<reads; i++,s+=step) {
    sum += (*data)[s];
  }
  return sum;
}

int main(int argc, char *argv[]) {
  PCM * m = PCM::getInstance();
  m->program(PCM::DEFAULT_EVENTS, NULL);

  if (m->program() != PCM::Success) {
    std::cout << "PMU not available, re-running will probably fix this" << std::endl;
    return 1;
  }

  if (argc < 3) {
    std::cout << "You need to provide the step size and number of reads as arguments" << std::endl;
    return 1;
  }
  std::istringstream iss(argv[1]);
  unsigned int step;
  iss >> step;
  std::istringstream isss(argv[2]);
  unsigned int reads;
  isss >> reads;

  if (reads*step > N) {
    std::cout << "Smaller step size/less reads needed." << std::endl;
  }

  auto d = prepare();
  
  SystemCounterState before = getSystemCounterState();

  auto s=test(d, step, reads);

  SystemCounterState after = getSystemCounterState();
  std::cout << "CPU cycles used: " << getRefCycles(before, after) <<
    "\nInstructions: " << getInstructionsRetired(before, after) <<
    "\nInstructions per Clock: " << getIPC(before, after) <<
    "\nL3 cache hit ratio: " << getL3CacheHitRatio(before, after) <<
    "\nL2 cache hit ratio: " << getL2CacheHitRatio(before, after) <<
    "\nWasted cycles caused by L3 misses: " << getCyclesLostDueL3CacheMisses(before, after) <<
    "\nBytes read from DRAM: " << getBytesReadFromMC(before, after) <<
    std::endl;
  
  std::cout << s << std::endl;
  m->cleanup();
}
