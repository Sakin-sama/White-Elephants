#include "helpers.hpp"
#include "autons.hpp"
//Arrays
    extern std::array<std::string, 10> autons;
    extern void (*autonFunctions[])();
//Structs
    struct autonSelectorOptions {
        /*
        Do NOT touch the assigned type   |           | change amount and  | do not change
                                         v           v names as needed    v given value
        */
        std::optional<pros::controller_digital_e_t> moveSelectionUp = std::nullopt;
        std::optional<pros::controller_digital_e_t> moveSelectionDown = std::nullopt;
        std::optional<pros::controller_digital_e_t> selectAuton = pros::E_CONTROLLER_DIGITAL_A;
    };
//Functions
    void autonSelector(const struct autonSelectorOptions& options = {});
    void printAutons();
    void autonSelectionUp(bool button);
    void autonSelectionDown(bool button);