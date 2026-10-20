#include "api.h"
#include "lemlib/api.hpp"

//Controllers
  extern pros::Controller master;

//Drivetrain Motors
  extern pros::MotorGroup intakeLeft;
  extern pros::MotorGroup intakeRight;

//Motors
  extern pros::Motor intake;
  extern pros::MotorGroup lift;
  
//Pneumatics
  extern pros::adi::Pneumatics clawLeft;
  extern pros::adi::Pneumatics clawRight;

//Sensors
  extern pros::Imu imu;
  extern pros::Distance left;
  extern pros::Distance right;

//Tracking Wheel
  extern pros::Rotation verticalEncoder;
  extern lemlib::TrackingWheel verticalTrackingWheel;
  extern pros::Rotation horizontalEncoder;
  extern lemlib::TrackingWheel horizontalTrackingWheel;

//Odom
  extern lemlib::OdomSensors Sensors;

//Drivetrain
  extern lemlib::Drivetrain Drivetrain;

//PID
  extern lemlib::ControllerSettings LateralController;
  extern lemlib::ControllerSettings AngularController;

//Input Curves
  extern lemlib::ExpoDriveCurve ThrottleCurve;
  extern lemlib::ExpoDriveCurve SteerCurve;

//Chassis
  extern lemlib::Chassis chassis;