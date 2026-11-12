#include "helpers.hpp"

bool currentlyIntaking = false;

double motorVelocity(int givenVelocity) {
  return (std::max(std::min(((givenVelocity * 127) / 100), 127), -127));
}

void motor_controls() {
	// Intake
	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
		if (currentlyIntaking) {
			intake.brake();
			currentlyIntaking = false;
		} else {
			intake.move(motorVelocity(85));
			currentlyIntaking = true;
		}
	}

// Lift Up
if ((master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) && (lift.get_position() < 1700)) {
	lift.move(100);
} else if ((master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) && (lift.get_position() > 0)) {
	lift.move(-100);
} else {
	lift.brake();
}
	
// 	// Lift Upcycle
// 	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
// 		if (lift.get_position() > 1440) {
// 			while(lift.get_position() > 0) {
// 				lift.move(-90);
// 			}
// 		}
// 		else {
// 			lift.move(90);
// 		}
// 	}
// 
// 	//Lift Downcycle
// 	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
// 		if (lift.get_position() < 0) {
// 			while(lift.get_position() < 1440) {
// 				lift.move(90);
// 			}
// 		}
// 		else {
// 			lift.move(-90);
// 		}
// 	}

	// Rotate Claw
	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
		claw.move_absolute(315.0, 100);
	} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
		claw.move_absolute(0.0, 100);
	} else {
		claw.brake();
	}
}


void pneumatic_controls() {
	//Claw
	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
		clawLeft.toggle();
		clawRight.toggle();
	}
}
