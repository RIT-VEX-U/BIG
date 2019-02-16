#include "main.h"
#include "hardware.h"
#include "external_functions.h"

using namespace pros;

//====================OPERATING CODE=====================
void moveLift(float speed)
{
	Hardware::upperLeftLiftMotor.move(speed);
	Hardware::upperRightLiftMotor.move(speed);
	Hardware::lowerLeftLiftMotor.move(speed);
	Hardware::lowerRightLiftMotor.move(speed);
}

float lift_up_speed = 127;
float lift_down_speed = 80;
float empty_hold_voltage = 10;
float full_hold_voltage = 30;

void moveLift(bool button_up, bool button_down)
{
	if(button_up)
	{
		moveLift(lift_up_speed);
	}else if(button_down)
	{
		moveLift(-lift_down_speed);
	}else if(Hardware::upperLeftLiftMotor.get_position() > 50)
	{
		moveLift(empty_hold_voltage);
	}else
	{
		moveLift(0);
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
 	{


	while(true)
	{
		//Driving Control
		driveMecanum(Hardware::controller1.get_analog(E_CONTROLLER_ANALOG_LEFT_Y), Hardware::controller1.get_analog(E_CONTROLLER_ANALOG_LEFT_X),Hardware::controller1.get_analog(E_CONTROLLER_ANALOG_RIGHT_X), true, 0.0);
		//End Driving Control

		//Operating Controls

		moveLift(Hardware::controller1.get_digital(E_CONTROLLER_DIGITAL_L1), Hardware::controller1.get_digital(E_CONTROLLER_DIGITAL_R1));

		//End Operating Controls


	}
	}
