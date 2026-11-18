#pragma once
#include "globals.hpp"
#include "pros/misc.h"
#include "autons.hpp"

//Variables
    extern bool isIntaking;
    extern float drivetrainSpeedConstant;
    extern int autonSelection;
    extern int autonSelectionScrollAmount;
//Arrays
    extern std::array<std::string, 10> autons;
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
    struct autonSelectorOptions {
        pros::controller_digital_e_t moveUp = pros::E_CONTROLLER_DIGITAL_UP;
        pros::controller_digital_e_t moveDown = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t selectAuton = pros::E_CONTROLLER_DIGITAL_A;
    };
//Functions
    double motorVelocity(int givenVelocity);
    float drivetrainSpeedModifier(pros::controller_digital_e_t button);
    void motorControls(const struct motorControlOptions options = {}, const struct motorSpeedOptions speedOptions = {});
    void intakeControl(bool forward, bool backward, double speed);
    void liftControl(bool up, bool down, double speed);
    void clawControl(bool up, bool down, double speed);
    void pneumaticControls();
    void autonSelector(const struct autonSelectorOptions options = {});