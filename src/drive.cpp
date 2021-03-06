#include "external_functions.h"
#include "hardware.h"

#define WHEEL_SIZE 4.0
#define TICKS_PER_REV 900.0

const float inchesPerTick = (WHEEL_SIZE * 3.14159) / TICKS_PER_REV;

void resetEncoders()
{
	Hardware::frontLeftMotor.tare_position();
	Hardware::frontRightMotor.tare_position();
	Hardware::backLeftMotor.tare_position();
	Hardware::backRightMotor.tare_position();
}

float getAbsValAvgEncVal()
{
	return (fabs((Hardware::frontLeftMotor.get_position()) * inchesPerTick)
	 + fabs((Hardware::backLeftMotor.get_position()) * inchesPerTick)
	 + fabs((Hardware::frontRightMotor.get_position()) * inchesPerTick)
	 + fabs((Hardware::backRightMotor.get_position()) * inchesPerTick)) / 4.0;
}

void drive(int leftMotor, int rightMotor)
{
	Hardware::frontLeftMotor.move(leftMotor);
	Hardware::backLeftMotor.move(leftMotor);
	Hardware::frontRightMotor.move(rightMotor);
	Hardware::backRightMotor.move(rightMotor);
}

/**
 *	Drives the robot using a tank-system.
 *
 *	Squaring the inputs allows slower control over the majority of the
 *	joystick band, but still allows fast movements near the end, following
 		a x^2 curve. Leave as false for a y=x linear function.
 */
void driveTank(float left, float right, bool square_inputs)
{

float left_out = left / 127.0;
float right_out = right / 127.0;

int left_sign = (left_out > 0) ? 1 : (left_out < 0) ? -1 : 0;
int right_sign = (right_out > 0) ? 1 : (right_out < 0) ? -1 : 0;

if(square_inputs)
{
	left_out = pow(left_out, 2) * left_sign;
	right_out = pow(right_out, 2) * right_sign;
}

drive((int)(left_out * 127), (int)(right_out * 127));
}

/**
* Drives the robot with a mecanum drivesystem, and with joystick inputs, between
* -127 and +127.
*
* If you set SquareInputs to true, you gain more control over small movements of
* the robot.
*
* Deadband is the percentage of joystick that the robot will not move, so that the
* motors aren't always receiving an input, and are constantly running. 0.1 is usually good.
*/
void driveMecanum(int leftYJoy, int leftXJoy, int rightXJoy, bool squareInputs, float deadband)
{
	float leftYIn = leftYJoy / 127.0;
	float leftXIn = leftXJoy / 127.0;
	float rightXIn = rightXJoy / 127.0;

	float magnitude = sqrt(pow((float)leftYIn, 2) + pow((float)leftXIn, 2));
	float direction = atan2(leftXIn, leftYIn);
	float rotation = rightXIn;

	driveMecanumRaw(magnitude, direction, rotation, squareInputs,deadband);
}

/**
*Drives the robot with a mecanum drive system, and direct magnitude / direction
* / rotation controls. Useful for auto.
* NOTE: set deadband and square inputs to 0 if you are running in auto.
*/
void driveMecanumRaw(float magnitude, float direction, float rotation, bool squareInputs, float deadband)
{
	//The wheels on the corner of the robot use the same formula, besides rotation.
	//Rotation is dertermined by whether the wheel is on the left or right side.
	float frontLeftOut = (magnitude * cos(direction - (3.14159 / 4.0))) + rotation;
	float backRightOut = (magnitude * cos(direction - (3.14159 / 4.0))) - rotation;
	float frontRightOut = (magnitude * cos(direction + (3.14159 / 4.0))) - rotation;
	float backLeftOut = (magnitude * cos(direction + (3.14159 / 4.0))) + rotation;
	//Limit the output between -1 and 1
	frontLeftOut = (frontLeftOut > 1) ? 1 : (frontLeftOut < -1) ? -1 : frontLeftOut;
	frontRightOut = (frontRightOut > 1) ? 1 : (frontRightOut < -1) ? -1 : frontRightOut;
	backLeftOut = (backLeftOut > 1) ? 1 : (backLeftOut < -1) ? -1 : backLeftOut;
	backRightOut = (backRightOut > 1) ? 1 : (backLeftOut < -1) ? -1 : backRightOut;

	if(squareInputs)
	{
		magnitude = (magnitude > 0 ? 1 : -1) * pow(magnitude, 2);
		rotation = (rotation > 0 ? 1 : -1) * pow(rotation, 2);
	}

		Hardware::frontLeftMotor.move(frontLeftOut * 127);
		Hardware::frontRightMotor.move(frontRightOut * 127);
		Hardware::backLeftMotor.move(backLeftOut * 127);
		Hardware::backRightMotor.move(backRightOut * 127);

}

bool driveInchesInit = true;

/**
	Drives the robot X inches in "degrees" direction, 0 being forward, positive 90
	to the right, and "magnitude" speed
*/
bool driveInches(float inches, float direction, float magnitude)
{
	if(driveInchesInit)
	{
		resetEncoders();
		driveInchesInit = false;
	}

	if(getAbsValAvgEncVal() > fabs(inches))
	{
		drive(0,0);
		driveInchesInit = true;
		return true;
	}

	driveMecanumRaw(magnitude, direction * (3.14159 / 180.0), 0, false, 0);

	return false;
}

bool turnInit = true;

/**
	turns the robot a certain number of degrees, at a certain speed.
	Positive degrees is clockwise and negative is counter-clockwise.
	Speed is between -1.0 ad 1.0
*/
bool turnDegrees(float degrees, float speed)
{
  if(turnInit)
	{
		resetEncoders();
		turnInit = false;
	}

	if(getAbsValAvgEncVal() >= fabs(degrees * (3.14159/180.0) * 12))
	{
		drive(0,0);
		turnInit = true;
		return true;
	}

	speed = fabs(127 * speed);
	if(degrees >= 0)
		drive(speed, -speed);
	else if(degrees < 0)
		drive(-speed, speed);

	return false;
}
