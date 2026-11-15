#pragma once
#include "globals.hpp"

//Functions
double motorVelocity(int givenVelocity);
float drivetrainSpeedModifier(bool chosenButton);
void motor_controls();
void intakeControl();
void liftControl();
void clawControl();
void pneumatic_controls();
//Variables
extern bool isIntaking;
extern float drivetrainSpeedConstant;