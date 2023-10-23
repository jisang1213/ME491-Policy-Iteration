#include <iostream>
#include "20190837.hpp"

int main() {
  Eigen::Vector<int, 12> state;
  state << 1,0,0, 0,1,1, 0,0,1, 0,0,0;

  std::cout << "optimal value for the state: " << getOptimalValue(state) << std::endl;
  std::cout << "optimal action for the state: " << getOptimalAction(state) << std::endl;

  return 0;
}
