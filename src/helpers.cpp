#include "helpers.hpp"
#include "pros/misc.h"

//Variables
bool isIntaking = false;
float drivetrainSpeedConstant = 0.85;
float currentDriveSpeedMultiplier = 1.0;
int autonSelection = 0;
int autonSelectionScrollAmount = 0;
//Arrays
std::array<std::string, 4> autons = {
    "None",
    "Teamwork",
    "Skills full",
    "Skills park"
};

void (*autonFunctions[])() = {
	nullptr,
	teamworkAuton,
	skillsAuton,
	skillsParkAuton
};

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
		while (!master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
			if (autons.size() > 0) {
				//Prints autons
				pros::lcd::clear();
				for (int i = 0; i < std::min(autons.size(), size_t(5)); i++) {
					if (autonSelection == i) {
						pros::lcd::print(i,">%s",autons[i + autonSelectionScrollAmount].c_str());
					} else {
						pros::lcd::print(i," %s",autons[i + autonSelectionScrollAmount].c_str());
					}
				}
				//Moves selection up
				if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
					if (autonSelection > 0) {
						autonSelection--;
					} else if (autonSelectionScrollAmount > 0) {
						autonSelectionScrollAmount--;
					} else {
						autonSelection = std::min(autons.size(), size_t(5)) - 1;
						autonSelectionScrollAmount = std::max(int(autons.size()) - 5, 0);
					}
				}
				//Moves selection down
				if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
					if (autonSelection < std::min(autons.size(), size_t(5)) - 1) {
						autonSelection++;
					} else if (5 + autonSelectionScrollAmount < autons.size()) {
						autonSelectionScrollAmount++;
					} else {
						autonSelection = 0;
						autonSelectionScrollAmount = 0;
					}
				}
			} else {
				//No autons found
				pros::lcd::clear();
				pros::lcd::print(0,"No autons found");
				pros::lcd::print(1,":|");
			}
			pros::delay(50);
		}
	}

