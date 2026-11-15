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

void motor_controls(struct motorControlOptions options) {
	//Intake (part of motor controls)
		if (options.intakeForward) {
			if (isIntaking) {
				intake.brake();
				isIntaking = false;
			} else {
				intake.move(motorVelocity(85));
				isIntaking = true;
			}
		} else if (options.intakeBackward) {
			while (options.intakeBackward) {
				intake.move(motorVelocity(-85));
			}
			if (isIntaking) {
				intake.move(motorVelocity(85));
			} else {
				intake.brake();
			}
		}
	
	//Lift (part of motor controls)
		if ((options.liftUp) && (lift.get_position() < 1700)) {
			lift.move(100);
		} else if ((options.liftDown) && (lift.get_position() > 0)) {
			lift.move(-100);
		} else {
			lift.brake();
		}

	// Rotate Claw (part of motor controls)
		if (options.clawUp) {
			claw.move_absolute(315.0, 100);
		} else if (options.clawDown) {
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
