#include <iostream>
#include <vector>

#include <blaze/Math.h>

#include "timer.hpp"

#define TIMESTEP (8.5/20)

typedef blaze::StaticMatrix<float,1,1> Obs;
typedef blaze::StaticMatrix<float,2,2> M2x2;
typedef blaze::StaticMatrix<float,2,1> M1x2;
typedef blaze::StaticMatrix<float,1,2> M2x1;

typedef M1x2 State;


void kalman(State& x, M2x2& P, Obs& obs) {
  // Performs one step of the Kalman filter.
  // modifies `x` and `P` in place
  
  
  //M2x2 I(1.f, 0.f, 0.f, 1.f); // identity matrix
  Obs R(10.f); // measurement noise, same shape as H*x
  M2x2 Q(0.f, 0.f, 0.f, 0.f); // motion/control noise
  // xprime = F*x, evolve state to next timestep
  M2x2 F(1.f, TIMESTEP, 0.f, 1.f);
  M2x1 H(1.f, 0.f); // obs = H*x
  // motion represents the 'forcing' applied to the system
  // also referred to as control input
  State motion(-0.5f*9.81f*(TIMESTEP*TIMESTEP),
	       -9.81*TIMESTEP);

  x = F*x + motion;
  // adding Q costs about 10% in performance with -O3
  // as it is zero here we just don't add it
  P = F*P*trans(F);// + Q;

  auto y = obs - H*x;
  auto S = H*P*trans(H) + R; // residual covariance
  auto K = P*(trans(H) / S(0,0)); // kalman gain or ratio
  x += K*y;
  P = (P - K*H*P); // is faster than
  //P = (I - K*H)*P;
}

int main(void) {
  std::vector<float> observations_{324.0f,
      321.66892961877016f,
      328.41069268423803f,
      313.22669905479245f,
      311.175345678846f,
      290.4838726021275f,
      293.39508840096545f,
      282.00300675061413f,
      281.2656654059701f,
      252.31603779890202f};
  std::vector<Obs> observations;
  for (auto o : observations_) {
    observations.push_back(Obs(o));
  }

#define ITER 1000000
  Timer t;
  t.start();
  for (auto i=0; i<ITER; ++i) {
    // initial state, heigh above ground and velocity
    State state(344.f, 0.f);
    // covariance matrix
    M2x2 P(20.f, 0.f,
	   0.f, 20.f);
    
    for (auto obs : observations) {
      kalman(state, P, obs);
    }
    
    auto s = state(0,0) + state(0,1);
    auto m = P(0,0) + P(0,1) + P(1,0) + P(1,1);
    doNotOptimizeAway(s);
    doNotOptimizeAway(m);
  }
  std::cout << "time taken: " <<t.stop()/ITER <<"ns"<<std::endl;
  return 0;
}
