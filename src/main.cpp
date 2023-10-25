#include <iostream>
#include "20190837.hpp"
//std::cout<<"111"<<std::endl;
int main() {
  Eigen::Vector<int, 12> state;
  state << 0,1,1, 1,1,1, 1,1,1, 1,1,1;

  std::cout << "optimal value for the state: \n" << getOptimalValue(state) << std::endl;
  std::cout << "optimal action for the state: \n" << getOptimalAction(state) << std::endl;

  return 0;
}
