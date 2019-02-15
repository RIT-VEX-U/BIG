
using namespace pros;

Motor frontLeftMotor = Motor(12, false);
Motor backLeftMotor = Motor(11, false);
Motor frontRightMotor = Motor(14, true);
Motor backRightMotor = Motor(13, true);

Motor upperLeftLiftMotor = Motor(2, true);
Motor lowerLeftLiftMotor = Motor(1, true);
Motor upperRightLiftMotor = Motor(4, false);
Motor lowerRightLiftMotor = Motor(3, false);

Controller controller1 = Controller(E_CONTROLLER_MASTER);
