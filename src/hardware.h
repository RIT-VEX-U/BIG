#ifndef HARDWARE
#define HARDWARE
#include "api.h"

using namespace pros;

class Hardware
{
public:
  static Motor frontLeftMotor;
  static Motor backLeftMotor;
  static Motor frontRightMotor;
  static Motor backRightMotor;

  static Motor upperLeftLiftMotor;
  static Motor lowerLeftLiftMotor;
  static Motor upperRightLiftMotor;
  static Motor lowerRightLiftMotor;

  static ADIGyro gyro;

  static Controller controller1;
};
#endif
