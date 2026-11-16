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
	void intakeControl(bool forward, bool backward, double speed) {
		if (forward) {
			if (isIntaking) {
				isIntaking = false;
			} else {
				isIntaking = true;
			}
		} else if (backward) {
			intake.move(-speed);
		} else {
			if (isIntaking) {
				intake.move(speed);
			} else {
				intake.brake();
			}
		}
	}

//Lift function
	void liftControl(bool up, bool down, double speed) {
		if (up) {
			lift.move_absolute(1700.0, speed);
		} else if (down) {
			lift.move_absolute(0.0, -speed);
		} else {
			lift.brake();
		}
	}

//Claw function
	void clawControl(bool up, bool down, double speed) {
		if (up) {
			claw.move_absolute(315.0, speed);
		} else if (down) {
			claw.move_absolute(0.0, -speed);
		} else {
			claw.brake();
		}
	}

//Motor controls
	void motorControls(struct motorControlOptions options, struct motorSpeedOptions speedOptions) {
		intakeControl(
			options.intakeForward,
			options.intakeBackward,
			motorVelocity(speedOptions.intakeSpeed)
		);
		
		liftControl(
			options.liftUp,
			options.liftDown,
			motorVelocity(speedOptions.liftSpeed)
		);

		clawControl(
			options.clawUp,
			options.clawDown,
			motorVelocity(speedOptions.clawSpeed)
		);
	}

//Pneumatic controls
	void pneumaticControls() {
		//Claw
			if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
				clawLeft.toggle();
				clawRight.toggle();
			}
	}

//Auton selector
	void autonSelector() {

	}
