#pragma config(Sensor, in1,    gyro,           sensorAnalog)
#pragma config(Motor,  port2,           L,             tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           R,             tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"
#include "gyro.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
  // ..........................................................................
  // Insert user code here.
  // ..........................................................................

  // Remove this function call once you have "real" code.
  AutonomousCodePlaceholderForTesting();
}

void drive()
{
	//Grab the user input
  	byte leftY = vexRT[Ch3];
  	byte rightY = vexRT[Ch2];

  	//Set the deadband on the joysticks, to stop continuous motion
  	if(fabs(leftY) > 20 || fabs(rightY) > 20)
  	{
  		//Convert -127 to 127 into -1.0 to 1.0, makes computations easier
  		//Square the inputs to give a slower acceleration, easier to control at low speeds
  		float leftOut = sgn(leftY) * pow((leftY) / 127.0, 2);
  		float rightOut = sgn(rightY) * pow((rightY) / 127.0, 2);

  		//Set the motors
  		motor[L] = leftOut * 127;
  		motor[R] = rightOut * 127;

  		writeDebugStreamLine("L %f", leftY);
  		writeDebugStreamLine("R %f", rightY);
  	}
  	else
  	{
  		motor[L] = 0;
  		motor[R] = 0;
  	}
}

float gyro_val = 0;

void getGyro()
{
	int gyro_raw = SensorValue[gyro];
	float gyro_accel = ((gyro_raw - 127) / 0.4458041958041958041958041958042) / 125.0;
	gyro_val -= gyro_accel;

writeDebugStreamLine("GyroOut: %f", gyro_val);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
  // User control code here, inside the loop
  while (true)
  {

  	drive();
  	getGyro();

    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
  }
}
