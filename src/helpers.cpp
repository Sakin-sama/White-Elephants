#include "helpers.hpp"
#include "liblvgl/llemu.hpp"
#include "pros/misc.h"

//Variables
bool isIntaking = false;
float drivetrainSpeedConstant = 0.85;
float currentDriveSpeedMultiplier = 1.0;
int autonSelection = 0;
int autonSelectionScrollAmount = 0;
//Arrays
std::array<std::string, 10> autons = {
	//Stays none
    "None",
	//Set to name of corresponding function
    "Teamwork",
    "Skills full",
    "Skills park",
	"N/A 1",
	"N/A 2",
	"N/A 3",
	"N/A 4",
	"N/A 5",
	"N/A 6",
};

void (*autonFunctions[])() = {
	//Stays nullptr
	nullptr,
	//Set to name of corresponding function
	teamworkAuton,
	skillsAuton,
	skillsParkAuton,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr
};

//Motor velocity conversion
	double motorVelocity(int givenVelocity) {
	return (std::max(std::min(((givenVelocity * 127) / 100), 127), -127));
	}

//Slowed driving
	float drivetrainSpeedModifier(pros::controller_digital_e_t chosenButton) {
		if (master.get_digital(chosenButton)) {
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
	void motorControls(const struct motorControlOptions options, const struct motorSpeedOptions speedOptions) {
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

//Print available autons
	void printAutons() {
		for (int i = 0; i < std::min(autons.size(), size_t(5)); i++) {
			pros::lcd::print(0, "Auton programs!");
			pros::lcd::print(1,":D");
			if (autonSelection == i) {
				pros::lcd::print(i + 2,">%s",autons[i + autonSelectionScrollAmount].c_str());
			} else {
				pros::lcd::print(i + 2," %s",autons[i + autonSelectionScrollAmount].c_str());
			}
		}	
	}

//Move auton selection up
	void autonSelectionUp(bool button) {
		if (button) {
			if (autonSelection > 0) {
				autonSelection--;
			} else if (autonSelectionScrollAmount > 0) {
				autonSelectionScrollAmount--;
			} else {
				autonSelection = std::min(autons.size(), size_t(5)) - 1;
				autonSelectionScrollAmount = std::max(int(autons.size()) - 5, 0);
			}
		}
	}

//Move auton selection down
	void autonSelectionDown(bool button) {
		if (button) {
			if (autonSelection < std::min(autons.size(), size_t(5)) - 1) {
				autonSelection++;
			} else if (5 + autonSelectionScrollAmount < autons.size()) {
				autonSelectionScrollAmount++;
			} else {
				autonSelection = 0;
				autonSelectionScrollAmount = 0;
			}
		}
	}

//Auton selector
	void autonSelector(const struct autonSelectorOptions options) {
		while (true) {
			//Prints autons
				printAutons();
			//Moves selection up
				autonSelectionUp(master.get_digital_new_press(options.moveUp));
			//Moves selection down
				autonSelectionDown(master.get_digital_new_press(options.moveDown));
			//Breaks function when auton is selected
				if (master.get_digital(options.selectAuton)) {
					pros::lcd::clear_line(6);
					pros::lcd::clear_line(7);
					break;
				}
			pros::delay(50);
		}
	}

