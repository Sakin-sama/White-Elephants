#pragma once
#include "globals.hpp"

//Variables
extern bool isIntaking;
extern float drivetrainSpeedConstant;
extern int autonSelection;
//Arrays
extern std::array<std::string, 4> autons;
extern void (*autonFunctions[])();
//Structs
struct motorControlOptions {
    int intakeForward = 0;
    int intakeBackward = 0;
    int liftUp = 0;
    int liftDown = 0;
    int clawUp = 0;
    int clawDown = 0;
};
struct motorSpeedOptions {
    double intakeSpeed = 127;
    double liftSpeed = 127;
    double clawSpeed = 127;
};
//Functions
double motorVelocity(int givenVelocity);
float drivetrainSpeedModifier(bool chosenButton);
void motorControls(struct motorControlOptions options = {}, struct motorSpeedOptions speedOptions = {});
void intakeControl(bool forward, bool backward, double speed);
void liftControl(bool up, bool down, double speed);
void clawControl(bool up, bool down, double speed);
void pneumaticControls();
int autonSelector();
void teamworkAuton();
void skillsAuton();
void skillsParkAuton();