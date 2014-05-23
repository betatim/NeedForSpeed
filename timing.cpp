// (C) 2014 Ben Couturier, Tim Head, Gerhard Raven
#include <iostream>
#include <chrono>
#include <cmath>


// Measure passage of time by simply using the timers
// which are part of the standard library
class Timer {
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
public:
  void start() {
    start_time = std::chrono::high_resolution_clock::now();
  }

  double stop() {
    auto stop_time = std::chrono::high_resolution_clock::now();
    return double(std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count());
  }
};

// Tricky inline ASM to measure passage of time precisely
#ifdef __i386
__inline__ uint64_t rdtsc() {
  uint64_t x;
  __asm__ volatile ("rdtsc" : "=A" (x));
  return x;
}
#elif __amd64
__inline__ uint64_t rdtsc() {
  uint64_t a, d;
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  return (d<<32) | a;
}
#endif
class tsc {
private:
  uint64_t start_time;
public:
  void start() { start_time = rdtsc(); }
  // Note: the 2.3 is the quoted clockspeed for my machine
  // You want to change this to your machines number
  double stop() {
    auto stop_time = rdtsc();
    return double(stop_time-start_time)/2.3;
  }
};


template <typename Timer_t>
double foo() {
  // by returning m we make sure the optimizer
  // does not completely eliminate the loop
  // try it out! The speed ups are fantastic
  // a good example of how you have to be
  // careful when measuring timing numbers!
  Timer_t x;
  double m;
  for (int k = 0; k< 100 ; ++k) {
    x.start();
    double j = 0;
    for (int i=0;i<100;++i) {
      j += sin(1.0*i);
    }
    m += j;
    auto t = x.stop();
    std::cout << "time : " << t << " ns" << std::endl;
  }
  return m;
}

int main() {
  // Claimed precision of the high resolution clock
  // on this machine. You can time with 1/den seconds
  // precision
  std::cout << std::chrono::high_resolution_clock::period::den << std::endl;
  
  // The conclusion here is probably that there is
  // no need to use TSC anymore, just use what is
  // provided in C++11 anyway
  std::cout << "C++11 timer" << std::endl;
  foo<Timer>();
  std::cout << "TSC timer" << std::endl;
  foo<tsc>(); 
}
