#include "helpers.hpp"

bool isIntaking = false;
float drivetrainSpeedConstant = 0.85;
float currentDriveSpeedMultiplier = 1.0;

//Motor velocity conversion
	double motorVelocity(int givenVelocity) {
	return (std::max(std::min(((givenVelocity * 127) / 100), 127), -127));
	}

//Slowed driving
	float drivetrainSpeedModifier(bool chosenButton) {
		if (chosenButton) {
			currentDriveSpeedMultiplier = drivetrainSpeedConstant;
			return drivetrainSpeedConstant;
		} else {
			currentDriveSpeedMultiplier = 1;
			return 1;
		}
	}

//Intake function
	void intakeControl() {
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			if (isIntaking) {
				isIntaking = false;
			} else {
				isIntaking = true;
			}
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
				intake.move(motorVelocity(-85));
		} else {
			if (isIntaking) {
				intake.move(motorVelocity(85));
			} else {
				intake.brake();
			}
		}
	}

//Lift function
	void liftControl() {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			lift.move_absolute(1700, motorVelocity(100));
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			lift.move_absolute(0, motorVelocity(-100));
		} else {
			lift.brake();
		}
	}

//Claw function
	void clawControl() {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			claw.move_absolute(315.0, motorVelocity(100));
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			claw.move_absolute(0.0, motorVelocity(-100));
		} else {
			claw.brake();
		}
	}

//Motor controls
	void motor_controls() {
		intakeControl();
		liftControl();
		clawControl();
	}

//Pneumatic controls
	void pneumatic_controls() {
		//Claw
			if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
				clawLeft.toggle();
				clawRight.toggle();
			}
	}
