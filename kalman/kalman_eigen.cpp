#include <iostream>
#include <vector>

#include <Eigen/Core>

#include "timer.hpp"

#define TIMESTEP (8.5/20)


/** Performs one step of the Kalman filter. Modifies `x` and `P` in place. */
void kalman(Eigen::Vector2f &x, Eigen::Matrix2f &P, const float &obs) {
  float R(10);
  Eigen::Matrix2f F;
  F << 1, TIMESTEP,
       0, 1;
  Eigen::RowVector2f H(1, 0);
  Eigen::Vector2f motion(-.5 * 9.81 * TIMESTEP * TIMESTEP, -9.81 * TIMESTEP);

  x = F * x + motion;
  P = F * P * F.transpose();

  auto y = obs - H * x;
  auto S = H * P * H.transpose() + R;
  auto K = P * H.transpose() / S;

  x += K * y;
  P = (P - K * H * P);
}

int main(int argc, char *argv[]) {
  std::vector<float> observations {
    324.0f,
    321.66892961877016f,
    328.41069268423803f,
    313.22669905479245f,
    311.175345678846f,
    290.4838726021275f,
    293.39508840096545f,
    282.00300675061413f,
    281.2656654059701f,
    252.31603779890202f
  };

  const unsigned int ITER = 1000000;
  Timer t;
  t.start();

  for (unsigned int i=0; i<ITER; i++) {
    Eigen::Vector2f state(344.f, 0.f);
    Eigen::Matrix2f P;  // Covariance matrix
    P << 20.f, 0.f,
         0.f, 20.f;
    for (const float &obs : observations) {
      kalman(state, P, obs);
    }

    auto s = state(0) + state(1);
    auto m = P(0, 0) + P(0, 1) + P(1, 0) + P(1, 1);
    doNotOptimizeAway(s);
    doNotOptimizeAway(m);
  }

  std::cout << "time taken: " << t.stop() / ITER << "ns" << std::endl;
}
