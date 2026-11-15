#include "helpers.hpp"

bool currentlyIntaking = false;
float drivetrainSpeedConstant = 0.85;

//Motor velocity conversion
	double motorVelocity(int givenVelocity) {
	return (std::max(std::min(((givenVelocity * 127) / 100), 127), -127));
	}

//Slowed driving
	float drivetrainSpeedModifier(bool chosenButton) {
		if (chosenButton) {
			return drivetrainSpeedConstant;
		} else {
			return 1;
		}
	}

void motor_controls() {
	//Intake (part of motor controls)
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			if (currentlyIntaking) {
				intake.brake();
				currentlyIntaking = false;
			} else {
				intake.move(motorVelocity(85));
				currentlyIntaking = true;
			}
		} else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			while (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
				intake.move(motorVelocity(-85));
			}
			if (currentlyIntaking) {
				intake.move(motorVelocity(85));
			} else {
				intake.brake();
			}
		}
	
	//Lift (part of motor controls)
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && (lift.get_position() < 1700)) {
			lift.move(100);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) && (lift.get_position() > 0)) {
			lift.move(-100);
		} else {
			lift.brake();
		}

	// Rotate Claw (part of motor controls)
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
