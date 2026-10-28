#include "main.h"


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
        // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
   	claw.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	
     //MVlib logger - straight from documentation example. See https://github.com/lewispinstein-hue/MotionView/blob/main/MVLib/README.md
   //   auto& logger = mvlib::Logger::getInstance();
   //   logger.setBuildDate(__DATE__);
   // 
   //   // Attach your odom 
   //   mvlib::setOdom(&chassis);
   //   // Needed for accurate speed telemetry
   //   logger.setRobot({
   //     .leftDrivetrain = &intakeLeft,
   //     .rightDrivetrain = &intakeRight
   //   });
   // 
   //   logger.start();

}

/**
 * Runs while the robot is disabled
 */
void disabled() {
  //autonSelector();
}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {
  //autonSelector();
}

// get a path used for pure pursuit
// this needs to be put outside a function
ASSET(example_txt); // '.' replaced with "_" to make c++ happy

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */
void autonomous() {
  //runSelectedAuton();
}

/**
 * Runs in driver control
 */
void opcontrol() {
  //Temporary code
  //Permanent code
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    while (true) {
      motor_controls();
      pneumatic_controls();
      // get joystick positions
      int leftY = 0.85 * master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
      int rightX = 0.85 * master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
      // move the chassis with curvature drive
      chassis.arcade(leftY, rightX);
      // delay to save resources
      pros::delay(10);
  }
}
