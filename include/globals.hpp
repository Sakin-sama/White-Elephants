
#include "api.h"
#include "lemlib/api.hpp"
#include "mvlib/Optional/lemlib.hpp"
#include <array>
#include <string>
#include <algorithm>
#include <optional>
  
//Controllers
  extern pros::Controller master;

//Drivetrain Motors


//Motors

  
//Pneumatics


//Sensors
  extern pros::Imu imu;

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
