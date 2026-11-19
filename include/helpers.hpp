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
    struct autonSelectorOptions {
        std::optional<pros::controller_digital_e_t> moveUp = std::nullopt;
        std::optional<pros::controller_digital_e_t> moveDown = std::nullopt;
        std::optional<pros::controller_digital_e_t> selectAuton = pros::E_CONTROLLER_DIGITAL_A;
    };
//Enums for classes
    enum class buttonPressTypes {
        HELD,
        NEW_PRESS,
        NEW_RELEASE
    };
//Functions
    double motorVelocity(int givenVelocity);
    float drivetrainSpeedModifier(const std::optional<pros::controller_digital_e_t>& button = std::nullopt);
    bool buttonStatus(const std::optional<pros::controller_digital_e_t>& button, const buttonPressTypes& pressType = buttonPressTypes::HELD);
    void motorControls(const struct motorControlOptions& options = {}, const struct motorSpeedOptions& speedOptions = {});
    void intakeControl(bool forward, bool backward, double speed);
    void liftControl(bool up, bool down, double speed);
    void clawControl(bool up, bool down, double speed);
    void pneumaticControls();
    void autonSelector(const struct autonSelectorOptions& options = {});