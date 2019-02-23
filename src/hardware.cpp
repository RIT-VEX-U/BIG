#include "hardware.h"
#include "api.h"

using namespace pros;

Motor Hardware::frontLeftMotor = Motor(11, false);
Motor Hardware::backLeftMotor = Motor(12, false);
Motor Hardware::frontRightMotor = Motor(13, true);
Motor Hardware::backRightMotor = Motor(14, true);

Motor Hardware::upperLeftLiftMotor = Motor(8, true);
Motor Hardware::lowerLeftLiftMotor = Motor(7, true);
Motor Hardware::upperRightLiftMotor = Motor(9, false);
Motor Hardware::lowerRightLiftMotor = Motor(10, false);

Motor Hardware::firstStageLeftLiftMotor = Motor(1, false);
Motor Hardware::firstStageRightLiftMotor = Motor(4, true);

Motor Hardware::punchMotor = Motor(6, false);

Motor Hardware::capFlipperMotor = Motor(3, false);

ADIGyro Hardware::gyro = ADIGyro(1, .1);
Controller Hardware::controller1 = Controller(E_CONTROLLER_MASTER);
