#include "helpers.hpp"


double motorVelocity(int givenVelocity) {
  return (std::max(std::min(((givenVelocity * 127) / 100), 127), -127));
}

void motor_controls() {
	
}