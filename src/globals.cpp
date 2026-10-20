#include "globals.hpp"

// INTAKE IS THE FRONT

//Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

//Motors
  //Intake/Outake motors
    pros::Motor intake(2, pros::v5::MotorGears::blue);
    pros::Motor claw(6, pros::v5::MotorGears::green);
    pros::MotorGroup lift({10,-1}, pros::v5::MotorGears::green);
    
  //Drivetrain
    pros::MotorGroup intakeLeft({12,11}, pros::v5::MotorGears::green);
    pros::MotorGroup intakeRight({-19,-20}, pros::v5::MotorGears::green);

//Pneumatics
  //Claw - Left/Right is relative to INTAKE
    pros::adi::Pneumatics clawLeft('H', false);
    pros::adi::Pneumatics clawRight('A', false);


//Sensors
  pros::Imu imu(13);
  pros::Distance left(11);
  pros::Distance right(11);
  pros::Distance clawSide(8);
  pros::Distance intakeSide(7);

//Tracking Wheels
  pros::Rotation verticalEncoder(-18);
  lemlib::TrackingWheel verticalTrackingWheel(&verticalEncoder, lemlib::Omniwheel::NEW_2, -0.75);

  pros::Rotation horizontalEncoder(-14);
  lemlib::TrackingWheel horizontalTrackingWheel(&horizontalEncoder, lemlib::Omniwheel::NEW_2, -0.75);

//Odometry Settings
  lemlib::OdomSensors Sensors(
    &verticalTrackingWheel, // vertical tracking wheel 1, set to null
    nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
    &horizontalTrackingWheel, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
    &imu // inertial sensor
    );

//Drivetrain
  lemlib::Drivetrain Drivetrain(
    &intakeLeft,
    &intakeRight,
    12.875, //measure
    lemlib::Omniwheel::NEW_325, //idk if it's new or old 325
    450, //gear ratio  * gear cartridge (200)
    8 //idk
  );

//PID - these are the default constants, need to be callibrated
  // angular PID controller
    lemlib::ControllerSettings AngularController(
        2, // proportional gain (kP)
        0, // integral gain (kI)
        10, // derivative gain (kD)
        0, // anti windup
        0, // small error range, in degrees
        0, // small error range timeout, in milliseconds
        0, // large error range, in degrees
        0, // large error range timeout, in milliseconds
        0 // maximum acceleration (slew)
    );

  // lateral PID controller
    lemlib::ControllerSettings LateralController(
        7, // proportional gain (kP)
        0, // integral gain (kI)
        15, // derivative gain (kD)
        0, // anti windup
        0, // small error range, in inches
        0, // small error range timeout, in milliseconds
        0, // large error range, in inches
        0, // large error range timeout, in milliseconds
        0 // maximum acceleration (slew)
    );

//Input Curves
  // input curve for throttle input during driver control
    lemlib::ExpoDriveCurve ThrottleCurve(
      3, // joystick deadband out of 127
      10, // minimum output where drivetrain will move out of 127
      1.019 // expo curve gain
    );
// input curve for steer input during driver control
    lemlib::ExpoDriveCurve SteerCurve
      (3, // joystick deadband out of 127
      10, // minimum output where drivetrain will move out of 127
      1.019 // expo curve gain
      );

//Chassis
  lemlib::Chassis chassis(
    Drivetrain,
    LateralController,
    AngularController,
    Sensors,
    &ThrottleCurve,
    &SteerCurve
  );
