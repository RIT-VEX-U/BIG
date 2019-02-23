#include "main.h"
#include "hardware.h"
#include "external_functions.h"

using namespace pros;

//====================OPERATING CODE=====================

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
		driveMecanum(Hardware::controller1.get_analog(E_CONTROLLER_ANALOG_LEFT_Y),
     Hardware::controller1.get_analog(E_CONTROLLER_ANALOG_LEFT_X),
     Hardware::controller1.get_analog(E_CONTROLLER_ANALOG_RIGHT_X), true, 0.0);
		//End Driving Control

		//Operating Controls

    operatorControls(Hardware::controller1.get_digital(E_CONTROLLER_DIGITAL_L1),
     Hardware::controller1.get_digital(E_CONTROLLER_DIGITAL_R1),
     Hardware::controller1.get_digital(E_CONTROLLER_DIGITAL_A));

     if(Hardware::controller1.get_digital(E_CONTROLLER_DIGITAL_B))
      setLiftHeight(32);

      if(Hardware::controller1.get_digital(E_CONTROLLER_DIGITAL_Y))
        Hardware::punchMotor.move(127);
      else
        Hardware::punchMotor.move(0);

		//End Operating Controls

		//lcd::print(0, "upper left: %f", Hardware::upperLeftLiftMotor.get_position());
    //lcd::print(1, "upper right: %f", Hardware::upperRightLiftMotor.get_position());
    //lcd::print(2, "lower left: %f", Hardware::lowerLeftLiftMotor.get_position());
    //lcd::print(3, "lower right: %f", Hardware::lowerRightLiftMotor.get_position());

    //lcd::print(0, "left: %f", Hardware::firstStageLeftLiftMotor.get_position());
    //lcd::print(1, "right: %f", Hardware::firstStageRightLiftMotor.get_position());

    lcd::print(0, "Lift Height: %f", getLiftHeight());
    lcd::print(1, "left enc: %f", Hardware::firstStageLeftLiftMotor.get_position());
    lcd::print(2, "right enc: %f", Hardware::firstStageRightLiftMotor.get_position());
    lcd::print(3, "flip encoder: %f", Hardware::capFlipperMotor.get_position());
		delay(50);

	}
	}
