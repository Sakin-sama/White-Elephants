#pragma once
#include "globals.hpp"

//Variables
extern bool isIntaking;
extern float drivetrainSpeedConstant;
//Structs
struct motorControlOptions {
    int intakeForward = 0;
    int intakeBackward = 0;
    int liftUp = 0;
    int liftDown = 0;
    int clawUp = 0;
    int clawDown = 0;
};
//Functions
double motorVelocity(int givenVelocity);
float drivetrainSpeedModifier(bool chosenButton);
void motor_controls(struct motorControlOptions options = {});
void pneumatic_controls();