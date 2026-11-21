#include "helpers.hpp"
#include "liblvgl/llemu.hpp"
#include "pros/misc.h"

//Variables
    bool isIntaking = false;
    float drivetrainSpeedConstant = 0.85;
    float currentDriveSpeedMultiplier = 1.0;

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
            if (lift.get_position() < 1690) {
			lift.move_absolute(1700.0, speed);
            return;
            }
            liftRotator.move_absolute(360.0,speed);
            return;
		}

        if (down) {
            if (liftRotator.get_position() > 10) {
			liftRotator.move_absolute(0.0, -speed);
            return;
            }
            lift.move_absolute(0.0,-speed);
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

