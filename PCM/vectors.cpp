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



int main(int argc, char *argv[]) {
  PCM * m = PCM::getInstance();
  m->program(PCM::DEFAULT_EVENTS, NULL);

  if (m->program() != PCM::Success) {
    std::cout << "PMU not available, re-running will probably fix this" << std::endl;
    return 1;
  }

#define N 100000

  SystemCounterState before = getSystemCounterState();

  std::vector<double> u;
  for (auto i=0; i<N; ++i) {
    u.insert(u.end(), 42.3);
  }

  SystemCounterState middle = getSystemCounterState();

  std::vector<double> v;
  for (auto i=0; i<N; ++i) {
    v.insert(v.begin(), 42.3);
  }

  SystemCounterState after = getSystemCounterState();
  std::cout << "Insert at the end" << std::endl;
  std::cout << "CPU Ref cycles used: " << getRefCycles(before, middle) <<
    "\nCPU core cycles used: " << getCycles(before, middle) <<
    "\nInstructions: " << getInstructionsRetired(before, middle) <<
    "\nInstructions per Clock: " << getIPC(before, middle) <<
    "\nL3 cache hit ratio: " << getL3CacheHitRatio(before, middle) <<
    "\nL2 cache hit ratio: " << getL2CacheHitRatio(before, middle) <<
    "\nWasted cycles caused by L3 misses: " << getCyclesLostDueL3CacheMisses(before, middle) <<
    "\nBytes read from DRAM: " << getBytesReadFromMC(before, middle) <<
    std::endl;

  std::cout << std::endl;
  std::cout << "Insert at the beginning" << std::endl;
  std::cout << "CPU Ref cycles used: " << getRefCycles(middle, after) <<
    "\nCPU core cycles used: " << getCycles(middle, after) <<
    "\nInstructions: " << getInstructionsRetired(middle, after) <<
    "\nInstructions per Clock: " << getIPC(middle, after) <<
    "\nL3 cache hit ratio: " << getL3CacheHitRatio(middle, after) <<
    "\nL2 cache hit ratio: " << getL2CacheHitRatio(middle, after) <<
    "\nWasted cycles caused by L3 misses: " << getCyclesLostDueL3CacheMisses(middle, after) <<
    "\nBytes read from DRAM: " << getBytesReadFromMC(middle, after) <<
    std::endl;
  
  m->cleanup();
  return 0;

}
