#pragma once
#include "globals.hpp"

//Functions
double motorVelocity(int givenVelocity);
void motor_controls();
void pneumatic_controls();
//Variables
extern bool isIntaking;
extern float drivetrainSpeedConstant;