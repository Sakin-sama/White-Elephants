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

/*
Motor velocity conversion
mostly for my own simplification
*/
	double motorVelocity(int givenVelocity) {
	    return (std::max(std::min(((givenVelocity * 127) / 100), 127), -127));
	}

/*
Slowed driving while holding a button
ideal for better control when desired
*can be turned off in main.cpp
(where joystick positions are taken)*
*/
	float drivetrainSpeedModifier(const std::optional<pros::controller_digital_e_t>& button) {
		if (buttonStatus(button)) {
			currentDriveSpeedMultiplier = drivetrainSpeedConstant;
			return drivetrainSpeedConstant;
		}

		currentDriveSpeedMultiplier = 1;
		return 1;
	}

/*
Checks if a button is pressed or even exists
then asks which type of button press we want
*/
    bool buttonStatus(const std::optional<pros::controller_digital_e_t>& button, const enum buttonPressTypes& pressType) {
        if (button) {
            if (pressType == buttonPressTypes::NEW_PRESS) {
                return master.get_digital_new_press(*button);
            }

            if (pressType == buttonPressTypes::NEW_RELEASE) {
                return master.get_digital_new_release(*button);
            }

            return master.get_digital(*button);
        }

        return false;
    }

/*
Function for intaking
*/
	void intakeControl(bool forward, bool backward, double speed) {
		if (forward) {
			isIntaking = !isIntaking;
            return;
		}

        if (backward) {
			intake.move(-speed);
            return;
		}

        if (isIntaking) {
            intake.move(speed);
            return;
        }

        intake.brake();
	}

/*
Function for your lift
*/
	void liftControl(bool up, bool down, double speed) {
		if (up) {
			lift.move_absolute(1700.0, speed);
            return;
		}

        if (down) {
			lift.move_absolute(0.0, -speed);
            return;
		}

		lift.brake();
	}

/*
Function for your claw
*/
	void clawRotatorControl(bool up, bool down, double speed) {
		if (up) {
			claw.move_absolute(315.0, speed);
            return;
		}

        if (down) {
			claw.move_absolute(0.0, -speed);
            return;
		}

		claw.brake();
	}

/*
Motor controls
gives on main area where all motor functions go
assign controls in main.cpp
*/
	void motorControls(const struct motorControlOptions& options, const struct motorSpeedOptions& speedOptions) {
		intakeControl(
			buttonStatus(options.intakeForward, buttonPressTypes::NEW_PRESS),
		    buttonStatus(options.intakeBackward),
			motorVelocity(speedOptions.intakeSpeed)
		);
		
		liftControl(
			buttonStatus(options.liftUp),
			buttonStatus(options.liftDown),
			motorVelocity(speedOptions.liftSpeed)
		);

		clawRotatorControl(
			buttonStatus(options.clawUp),
			buttonStatus(options.clawDown),
			motorVelocity(speedOptions.clawSpeed)
		);
	}

/*
Claw toggle function
*/
    void clawStateControl(bool toggle) {
        if (toggle) {
            clawLeft.toggle();
            clawRight.toggle();
        }
    }
/*
Pneumatic controls
gives on main area where all pneumatic functions go
assign controls in main.cpp
*/
	void pneumaticControls(const struct pneumaticControlOptions& options) {
        clawStateControl(
            buttonStatus(options.clawToggle, buttonPressTypes::NEW_PRESS)
        );
	}

/*
Print available autons
*/
	void printAutons() {
		pros::lcd::print(0, "Auton programs!");
		pros::lcd::print(1,":D");
		for (int i = 0; i < std::min(autons.size(), size_t(5)); i++) {
			if (autonSelection == i) {
				pros::lcd::print(i + 2,">%s",autons[i + autonSelectionScrollAmount].c_str());
			} else {
				pros::lcd::print(i + 2," %s",autons[i + autonSelectionScrollAmount].c_str());
			}
		}	
	}

/*
Move auton selection up
*/
	void autonSelectionUp(bool button) {
		if (button) {
			if (autonSelection > 0) {
				autonSelection--;
                return;
			}

            if (autonSelectionScrollAmount > 0) {
				autonSelectionScrollAmount--;
                return;
			}

            autonSelection = std::min(autons.size(), size_t(5)) - 1;
            autonSelectionScrollAmount = std::max(int(autons.size()) - 5, 0);
		}
	}

/*
Move auton selection down
*/
	void autonSelectionDown(bool button) {
		if (button) {
			if (autonSelection < std::min(autons.size(), size_t(5)) - 1) {
				autonSelection++;
                return;
			}

            if (5 + autonSelectionScrollAmount < autons.size()) {
				autonSelectionScrollAmount++;
                return;
			}

            autonSelection = 0;
            autonSelectionScrollAmount = 0;
		}
	}

/*
Auton selector
put auton names and functions in arrays at top of file
make sure names match with function placement in arrays
*/
	void autonSelector(const struct autonSelectorOptions& options) {
		while (true) {
			//Prints autons
				printAutons();
			//Moves selection up
				autonSelectionUp(buttonStatus(options.moveUp, buttonPressTypes::NEW_PRESS));
			//Moves selection down
				autonSelectionDown(buttonStatus(options.moveDown, buttonPressTypes::NEW_PRESS));
			//Breaks function when auton is selected
				if (buttonStatus(options.selectAuton)) {
					pros::lcd::clear_line(6);
					pros::lcd::clear_line(7);
					break;
				}
			pros::delay(50);
		}
	}

