#pragma once
#include "globals.hpp"
#include "pros/misc.h"

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
        /*
        Do NOT touch the assigned type   |           | change amount and  | do not change
                                         v           v names as needed    v given value
        */
        std::optional<pros::controller_digital_e_t> intakeForward = std::nullopt;
        std::optional<pros::controller_digital_e_t> intakeBackward = std::nullopt;
        std::optional<pros::controller_digital_e_t> liftUp = std::nullopt;
        std::optional<pros::controller_digital_e_t> liftDown = std::nullopt;
        std::optional<pros::controller_digital_e_t> clawUp = std::nullopt;
        std::optional<pros::controller_digital_e_t> clawDown = std::nullopt;
    };
    struct motorSpeedOptions {
        double intakeSpeed = 127;
        double liftSpeed = 127;
        double clawSpeed = 127;
    };
    
    struct pneumaticControlOptions {
        /*
        Do NOT touch the assigned type   |           | change amount and  | do not change
                                         v           v names as needed    v given value
        */
        std::optional<pros::controller_digital_e_t> clawToggle = std::nullopt;
    };
//Enums for classes
    enum class buttonPressTypes {
        HELD,
        NEW_PRESS,
        NEW_RELEASE
    };
//Functions (can make own, ask for help as needed)
    //Miscellaneous helpers
        double motorVelocity(int givenVelocity);
        float drivetrainSpeedModifier(const std::optional<pros::controller_digital_e_t>& button = std::nullopt);
        bool buttonStatus(const std::optional<pros::controller_digital_e_t>& button, const enum buttonPressTypes& pressType = buttonPressTypes::HELD);
    //Related to motors (use motorControls, do not remove it)
        void motorControls(const struct motorControlOptions& options = {}, const struct motorSpeedOptions& speedOptions = {});
        void intakeControl(bool forward, bool backward, double speed);
        void liftControl(bool up, bool down, double speed);
        void clawRotatorControl(bool up, bool down, double speed);
    //Related to pneumatics (use pneumaticControls, do not remove it)
        void pneumaticControls(const struct pneumaticControlOptions& options = {});
        void clawStateControl(bool toggle);