#include "autonSelector.hpp"
//Variables
    int autonSelection = 0;
    int autonSelectionScrollAmount = 0;
//Arrays
    std::array<std::string, 10> autons = {
        //Stays none
        "None",
        //Set to name of corresponding function
        "N/A 1",
        "N/A 2",
        "N/A 3",
        "N/A 4",
        "N/A 5",
        "N/A 6",
        "N/A 7",
        "N/A 8",
        "N/A 9"
    };

    void (*autonFunctions[])() = {
        //Stays nullptr
        nullptr,
        //Set to name of corresponding function
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
		nullptr
    };
/*
Print available autons
*/
	void printAutons() {
		pros::lcd::print(0, "Auton programs!");
		pros::lcd::print(1,":D");
		for (int i = 0; i < std::min(autons.size(), size_t(5)); i++) {
			if (autonSelection == i) {
				pros::lcd::print(i + 2,">%s",autons[i + autonSelectionScrollAmount].c_str());
			} else {
				pros::lcd::print(i + 2," %s",autons[i + autonSelectionScrollAmount].c_str());
			}
		}	
	}

/*
Move auton selection up
*/
	void autonSelectionUp(bool button) {
		if (button) {
			if (autonSelection > 0) {
				autonSelection--;
                return;
			}

            if (autonSelectionScrollAmount > 0) {
				autonSelectionScrollAmount--;
                return;
			}

            autonSelection = std::min(autons.size(), size_t(5)) - 1;
            autonSelectionScrollAmount = std::max(int(autons.size()) - 5, 0);
		}
	}

/*
Move auton selection down
*/
	void autonSelectionDown(bool button) {
		if (button) {
			if (autonSelection < std::min(autons.size(), size_t(5)) - 1) {
				autonSelection++;
                return;
			}

            if (5 + autonSelectionScrollAmount < autons.size()) {
				autonSelectionScrollAmount++;
                return;
			}

            autonSelection = 0;
            autonSelectionScrollAmount = 0;
		}
	}

/*
Auton selector
put auton names and functions in arrays at top of file
make sure names match with function placement in arrays
*/
	void autonSelector(const struct autonSelectorOptions& options) {
		while (true) {
			//Prints autons
				printAutons();
			//Moves selection up
				autonSelectionUp(buttonStatus(options.moveSelectionUp, buttonPressTypes::NEW_PRESS));
			//Moves selection down
				autonSelectionDown(buttonStatus(options.moveSelectionDown, buttonPressTypes::NEW_PRESS));
			//Breaks function when auton is selected
				if (buttonStatus(options.selectAuton)) {
					pros::lcd::clear_line(6);
					pros::lcd::clear_line(7);
					break;
				}
			pros::delay(50);
		}
	}