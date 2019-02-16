#include "hardware.h"
#include "api.h"

using namespace pros;

Motor Hardware::frontLeftMotor = Motor(1, false);
Motor Hardware::backLeftMotor = Motor(2, false);
Motor Hardware::frontRightMotor = Motor(3, true);
Motor Hardware::backRightMotor = Motor(4, true);

Motor Hardware::upperLeftLiftMotor = Motor(5, false);
Motor Hardware::lowerLeftLiftMotor = Motor(6, false);
Motor Hardware::upperRightLiftMotor = Motor(7, true);
Motor Hardware::lowerRightLiftMotor = Motor(8, true);

Motor Hardware::firstStageLeftLiftMotor = Motor(9, false);
Motor Hardware::firstStageRightLiftMotor = Motor(10, true);

Motor Hardware::capFlipperMotor = Motor(11, false);

ADIGyro Hardware::gyro = ADIGyro(1, .1);
Controller Hardware::controller1 = Controller(E_CONTROLLER_MASTER);
