#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

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


int main(void) {
  int N = 2*52428800; // about 100MB
  std::vector<char> data;
  data.reserve(N);

  // somewhere to read and write to fill
  // the cahces with irrelevant stuff
  std::vector<char> thrash;
  thrash.reserve(N);

  for (auto i=0; i<N; ++i) {
    data.push_back(i);
  }

  for (auto i=0; i<N; ++i) {
    thrash.push_back(i*3);
  }

  // these are only here to serve as a way of stopping
  // the optimiser from getting rid of the loop bodies
  // it is important that we return them at the end
  auto blah(0), sum(0);
  for (auto i=0; i<N; ++i) {
    blah += thrash[i];
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1000*2));
  Timer timer;

  for (auto step=1; step<3000; ++step) {
    int reads(0);
    timer.start();
    for (auto s=0; s<N; s+=step) {
      reads += 1;
      sum += data[s];
    }
    auto t = timer.stop();

    // do stuff with the rubbish array
    for (auto i=0; i<N; ++i) {
      blah += thrash[i];
    }
    t *= 1.e-9 * 1048576; // ns to seconds and bytes to MB
    std::cout << step <<" "<< reads/t <<std::endl;
  }

  return sum+blah;
}
