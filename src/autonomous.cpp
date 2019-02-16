#include "main.h"

#define CLAW_TO_CENTER_DIST 5

enum AutoPath
{
init, driveToCap1, pickupCap1, reverseFromCap1, spin1, strafeToPole1, driveToPole1,
placeCap1, reverseFromPole1, strafe2, spin2, driveToCap2, pickupCap2, reverseFromCap2,
turnToPole2, driveToPole2, placeCap2, reverseFromPole2, spin3, strafe3, waitForLittleBot,
driveToTopField, turnToCap3, driveToCap3, pickupCap3, spin4, driveFoward, strafeToPole3,
driveToPole3, placeCap3, reverseFromCap3, spin5, strafeToCap4, driveToCap4, flipCap4,
reverseFromCap4
};

AutoPath currentPath = init;

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

  while(true)
  {
    switch(currentPath)
    {
      case init:
        //Any initialization code for only autonomous

        currentPath = driveToCap1;
      break;
      case driveToCap1:

      break;
    }
  }
}
