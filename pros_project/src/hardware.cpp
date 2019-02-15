#include "hardware.h"
#include "api.h"

using namespace pros;

Motor Hardware::frontLeftMotor = Motor(10, false);
Motor Hardware::backLeftMotor = Motor(9, false);
Motor Hardware::frontRightMotor = Motor(1, true);
Motor Hardware::backRightMotor = Motor(2, true);

Motor Hardware::upperLeftLiftMotor = Motor(5, false);
Motor Hardware::lowerLeftLiftMotor = Motor(6, false);
Motor Hardware::upperRightLiftMotor = Motor(7, true);
Motor Hardware::lowerRightLiftMotor = Motor(8, true);

ADIGyro Hardware::gyro = ADIGyro(1, .1);
Controller Hardware::controller1 = Controller(E_CONTROLLER_MASTER);
