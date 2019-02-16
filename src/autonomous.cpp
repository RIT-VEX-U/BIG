#include "main.h"

#define CLAW_TO_CENTER_DIST 5
#define AUTO_DRIVE_SPEED .7
#define AUTO_TURN_SPEED .5

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
        if(driveInches(24 - CLAW_TO_CENTER_DIST, 0, AUTO_DRIVE_SPEED);
          currentPath = pickupCap1;
      break;
      case pickupCap1:
        if(setLiftHeight(4))
          currentPath = reverseFromCap1;
        break;
      case reverseFromCap1:
        if(driveInches(24 - CLAW_TO_CENTER_DIST, 180, AUTO_DRIVE_SPEED))
          currentPath = spin1;
        break;
      case spin1:
        if(turn(-180, AUTO_TURN_SPEED))
          currentPath = strafeToPole2;
        break;
      
    }
  }
}
