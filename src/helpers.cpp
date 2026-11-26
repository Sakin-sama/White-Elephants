#include "helpers.hpp"
#include "liblvgl/llemu.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include <cmath>

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

//PID exit condition
    bool exitConditionPID(const pros::Motor& motor, const double desiredPosition, const int time, const moveAbsoluteSensor sensor, const moveAbsoluteConditions exitConditions) {
        if (!exitConditions.isExitCondition) {
            return true;
        }

        if (exitConditions.timeoutExitCondition < time) {
            return false;
        }

        if (sensor.rotationSensor) {
            if (exitConditions.errorExitCondition > std::abs((desiredPosition - (sensor.rotationSensor->get_position()/100.0)))) {
                return false;
            }
            return true;
        }

        if (sensor.motorSensor) {
            if (exitConditions.errorExitCondition > std::abs((desiredPosition - (sensor.motorSensor->get_position()/100.0)))) {
                return false;
            }
            return true;
        }

        if (exitConditions.errorExitCondition > std::abs((desiredPosition - (motor.get_position()/100.0)))) {
            return false;
        }

        return true;
    }

//P value updates
    double returnP(const double desiredPosition, const double currentPosition) {
        return desiredPosition - currentPosition;
    }

//I value updates
    double returnI(const double previousI, const double previousPosition, const double currentPosition, const double desiredPosition) {
        bool crossedTarget;
        if (std::signbit(previousPosition - desiredPosition) == std::signbit(currentPosition - desiredPosition)) {
            crossedTarget = false;
        } else {
            crossedTarget = true;
        }

        if (crossedTarget) {
            return 0;
        }

        return previousI + (desiredPosition - currentPosition);
    }

//D value updates
    double returnD(const double currentPosition, const double previousPosition) {
        return (previousPosition - currentPosition)/0.025;
    }

/*
THIS FUNCTION THREADS
Do not call it for a singular motor multiple times unless certain
that the old task has ended by the new call
*/
    void moveAbsolute(const pros::Motor motor, const double desiredPosition, const moveAbsoluteSensor sensor, const struct moveAbsolutePID PID, const struct moveAbsoluteConditions exitConditions) {
        //Declares PID and gives starting values
            double previousPosition;
            double currentPosition;
            if (sensor.rotationSensor) {
                previousPosition = (sensor.rotationSensor->get_position()/100.0);
                currentPosition = (sensor.rotationSensor->get_position()/100.0);
            } else if (sensor.motorSensor) {
                previousPosition = sensor.motorSensor->get_position();
                currentPosition = sensor.motorSensor->get_position();
            } else {
                previousPosition = motor.get_position();
                currentPosition = motor.get_position();
            }
            double P;
            double I;
            double D;
            int time = 0;
        
        //Runs PID program
        while (exitConditionPID(motor, desiredPosition, time, sensor, exitConditions)) {
            //Runs PID calculations
                if (sensor.rotationSensor) {
                    currentPosition = sensor.rotationSensor->get_position()/100.0;
                } else if (sensor.motorSensor) {
                    currentPosition = sensor.motorSensor->get_position();
                } else {
                    currentPosition = motor.get_position();
                }
                P = returnP(desiredPosition, currentPosition);
                I = returnI(I, previousPosition, currentPosition, desiredPosition);
                D = returnD(currentPosition, previousPosition);
                previousPosition = currentPosition;
            
            //Uses PID on motor and gives small delay
                motor.move(motorVelocity((P * PID.kP) + (I * PID.kI) + (D * PID.kD)));
                pros::delay(25);
                time += 25;
        }
        motor.brake();
    }

//Function for intaking
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

//Function for your lift
	void liftControl(bool up, bool down, double speed) {
        if (up && ((liftRotatorPosition.get_position()/100.0) > 125)) {
            // lift.move_absolute(1080, speed);
            lift.move(speed);
            return;
        }

        if (down) {
            lift.move_absolute(0, speed);
            return;
        }

        lift.brake();
	}


//Function for your claw rotator
	void liftRotatorControl(bool up, bool down, double speed) {
        if (up) {
            liftRotator.move_absolute(540, speed);
            return;
        }

        if (down && (lift.get_position() > 25)) {
            liftRotator.move_absolute(140, speed);
            return;
        }

        if (down) {
            liftRotator.move_absolute(0,speed);
            return;
        }

        liftRotator.brake();
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

		liftRotatorControl(
			buttonStatus(options.liftRotatorUp),
			buttonStatus(options.liftRotatorDown),
			motorVelocity(speedOptions.liftRotatorSpeed)
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

