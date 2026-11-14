#pragma once
#include "globals.hpp"

//Functions
double motorVelocity(int givenVelocity);
void motor_controls();
void pneumatic_controls();
//Variables
extern bool currentlyIntaking;
extern float drivetrainSpeedConstant;