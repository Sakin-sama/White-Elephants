#include "helpers.hpp"

bool currentlyIntaking = false;

double motor_velocity(int givenVelocity) {
  return (std::max(std::min(((givenVelocity * 127) / 100), 127), -127));
}

void motor_controls() {
	// Intake
	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
		if (currentlyIntaking) {
			intake.brake();
			currentlyIntaking = false;
		} else {
			intake.move(motor_velocity(100));
			currentlyIntaking = true;
		}
	}

	// Lift Up
	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
		lift.move(motor_velocity(100));
	}

	//Lift Down
	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
		lift.move(motor_velocity(-100));
	}

	//Lift Brake
	if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_L1)) {
		lift.brake();
	} else if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_L2)) {
		lift.brake();
	}

	// Rotate Claw
	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
		claw.move(motor_velocity(100));
	}

	//Rotate Claw Down
	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
		claw.move(motor_velocity(-100));
	}

	//Claw Brake
	if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_R1)) {
		claw.brake();
	} else if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_R2)) {
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