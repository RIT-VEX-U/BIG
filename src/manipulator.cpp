#include "external_functions.h"
#include "hardware.h"
#include "api.h"

#define FIRST_STAGE_MIN 0
#define FIRST_STAGE_LOW 5
#define FIRST_STAGE_HIGH 790
#define FIRST_STAGE_MAX 830
#define SECOND_STAGE_MIN 0
#define SECOND_STAGE_LOW 5
#define SECOND_STAGE_HIGH 2000
#define SECOND_STAGE_MAX 2100
#define LIFT_HOLDING_POWER_SECOND_STAGE .2
#define LIFT_HOLDING_POWER_FIRST_STAGE .5
#define LIFT_HOLDING_DEADBAND 20

#define FIRST_STAGE_SPEED_UP 1
#define FIRST_STAGE_SPEED_DOWN -.1

#define SECOND_STAGE_SPEED_UP 1
#define SECOND_STAGE_SPEED_DOWN -.5

#define FIRST_STAGE_ANGLE_LOW 5
#define FIRST_STAGE_ANGLE_HIGH 180

#define SECOND_STAGE_ANGLE_LOW 20
#define SECOND_STAGE_ANGLE_HIGH 155

#define FIRST_STAGE_BAR_LENGTH 12
#define SECOND_STAGE_BAR_LENGTH 12

#define AUTO_LIFT_DEADBAND 1

#define CAP_FLIP_MIN 0
#define CAP_FLIP_MAX 450
#define CAP_FLIP_SPEED .7
#define CAP_FLIP_MIN_HEIGHT 12

/**
  Gets the average encoder value of the lift,
  in raw encoder units, given either the first stage
  or second stage.
*/
float getAverageEncoderValue(int liftStage)
{
  if(liftStage == 1)
    return (Hardware::firstStageLeftLiftMotor.get_position()
    + Hardware::firstStageRightLiftMotor.get_position()) / 2.0;

  else if(liftStage == 2)
    return (Hardware::upperRightLiftMotor.get_position() + Hardware::lowerRightLiftMotor.get_position()
     + Hardware::upperLeftLiftMotor.get_position() + Hardware::lowerLeftLiftMotor.get_position()) / 4.0;

  return 0;
}

/**
  Moves the lift using a selector and value scheme.
*/
void moveLift(int motorStage, float power)
{
  if(motorStage == 1)
  {
  Hardware::firstStageLeftLiftMotor.move(power * 127);
  Hardware::firstStageRightLiftMotor.move(power*127);
  }else if(motorStage == 2)
  {
  Hardware::upperLeftLiftMotor.move(power * 127);
  Hardware::upperRightLiftMotor.move(power * 127);
  Hardware::lowerLeftLiftMotor.move(power * 127);
  Hardware::lowerRightLiftMotor.move(power * 127);
  }
}

/**
* Moves the lift based on buttons: If the up button is pressed, then the lift
* first moves the first stage. When the first stage reaches it's maximum height,
* it stops it and moves the second stage until it reaches it's max height, and
* it stops. The opposite is what happens when the down is pressed.
*
* The lift also has a holding voltage, which is applied only between HIGH and LOW
* values of the lift, to avoid hitting hardware stops. The holding voltage is
* not applied between LOW and MIN to keep it on the ground.
*/
int moveLift(bool buttonUp, bool buttonDown)
{
  float stage1Avg = getAverageEncoderValue(1);
  float stage2Avg = getAverageEncoderValue(2);

  if(buttonUp)
  {
    // FIRST move the 1st stage up
    if(stage1Avg > FIRST_STAGE_MAX)
      moveLift(1, 0);
    else if(stage1Avg < FIRST_STAGE_HIGH)
      moveLift(1, FIRST_STAGE_SPEED_UP);
    else if(stage1Avg < FIRST_STAGE_MAX)
      moveLift(1, LIFT_HOLDING_POWER_FIRST_STAGE * (.5 * (-cos(2*(getLiftAngle(1) * (3.14159 / 180.0))) + 1)));

    // IF we see the 1st stage is up, THEN start moving the 2nd stage up.
    if(stage2Avg > SECOND_STAGE_MAX || stage1Avg < FIRST_STAGE_HIGH)
      moveLift(2, 0);
    else if(stage2Avg < SECOND_STAGE_HIGH && stage1Avg >= FIRST_STAGE_HIGH)
      moveLift(2, SECOND_STAGE_SPEED_UP);
    else if(stage2Avg < SECOND_STAGE_MAX)
      moveLift(2, LIFT_HOLDING_POWER_SECOND_STAGE * (.5 * (-cos(2*(getLiftAngle(2) * (3.14159 / 180.0))) + 1)));

  }else if(buttonDown)
  {
    // FIRST move the 2nd stage down
    if(stage2Avg <= SECOND_STAGE_MIN)
      moveLift(2, 0);
    else if(stage2Avg > SECOND_STAGE_LOW)
      moveLift(2, SECOND_STAGE_SPEED_DOWN);

    // IF the 2nd stage is down, THEN start moving the 1st stage down.
    if(stage1Avg < FIRST_STAGE_MIN)
      moveLift(1, 0);
    if(stage2Avg > SECOND_STAGE_LOW)
      moveLift(1, LIFT_HOLDING_POWER_FIRST_STAGE * (.5 * (-cos(2*(getLiftAngle(1) * (3.14159 / 180.0))) + 1)));
    else if(stage1Avg > FIRST_STAGE_LOW)
      moveLift(1, FIRST_STAGE_SPEED_DOWN);
  }else
  {
    // Uses a sin wave to distribute the power required to keep the lift at that angle.
    // As the lift reaches 90 degrees, power increases, and as it reaches 180, it decreases.
    moveLift(1, LIFT_HOLDING_POWER_FIRST_STAGE * (.5 * (-cos(2*(getLiftAngle(1) * (3.14159 / 180.0))) + 1)));

    // Same for the 2nd stage.
    moveLift(2, LIFT_HOLDING_POWER_SECOND_STAGE * (.5 * (-cos(2*(getLiftAngle(2) * (3.14159 / 180.0))) + 1)));

  }

  if(stage1Avg < FIRST_STAGE_LOW)
    return 0;
  else if(stage1Avg < FIRST_STAGE_HIGH)
    return 1;
  else if(stage2Avg < SECOND_STAGE_HIGH)
    return 2;
  else
    return 3;

}

/**
  Gets the angle of each stage of the lift mechanism, from the vertical axis.
*/
float getLiftAngle(int liftStage)
{
  if(liftStage == 1)
    return ((FIRST_STAGE_ANGLE_HIGH - FIRST_STAGE_ANGLE_LOW)
     * (getAverageEncoderValue(1) / (FIRST_STAGE_MAX - FIRST_STAGE_MIN)))
     + FIRST_STAGE_ANGLE_LOW;
  else if(liftStage == 2)
  return ((SECOND_STAGE_ANGLE_HIGH - SECOND_STAGE_ANGLE_LOW)
   * (getAverageEncoderValue(2) / (SECOND_STAGE_MAX - SECOND_STAGE_MIN)))
   + SECOND_STAGE_ANGLE_LOW;
}

/**
 Gets the height of the lift, in inches using this method:
 1. Use y=mx + b, where y is the height of a stage, m is the range of motion in
    degrees(max angle - min angle), x is the percentage of the lift motion
    (currentLift / (max ticks - min ticks)), between 0 and 1.0, and b is the
    minimum amount of degrees. Congrats, you have the angle of that stage.
 2. We are getting the vertical component of this. Normally, to get vertical velocity
    around a circle, we use mag*sin(x). We need position, so we take the integral,
    which is -cos(x). Fit this to between 0 and 2 by using -(cos(x) + 1), and multiply
    it by the bar length.
 3. Add the first to the second stage, and poof!
*/
float getLiftHeight()
{
  float stage1Angle = getLiftAngle(1);
  float stage2Angle = getLiftAngle(2);

  float stage1Height = FIRST_STAGE_BAR_LENGTH * (-cos(stage1Angle * (3.14159 / 180.0)) + 1);
  float stage2Height = SECOND_STAGE_BAR_LENGTH * (-cos(stage2Angle * (3.14159 / 180.0)) + 1);

   return stage1Height + stage2Height;
}

/**
  Automatically sets the height of the lift from the ground using default speeds.
  If the lift is below the setpoint, move up. If it is above, move down. When it
  reaches it's destination, it returns true.
*/
bool setLiftHeight(float inches)
{
  float liftHeight = getLiftHeight();
  if(fabs(inches - liftHeight) > AUTO_LIFT_DEADBAND)
    {
      moveLift(false, false);
      return true;
    }

  if(liftHeight < inches)
    moveLift(true, false);
  else if(liftHeight > inches)
    moveLift(false, true);

  return false;
}

int flipCapStage = 4;
bool capWasOnGround = false;
/**
  Flips the cap automatically.
  If the lift is not above a certain height, raise it, flip and put it down.
  DO NOT use this and moveLift(bool, bool) at the same time. BAD STUFF WILL HAPPEN!
*/
bool flipCap(bool button)
{
  float currentLiftHeight = getLiftHeight();

  if(button)
    flipCapStage = 0;

  switch(flipCapStage)
  {
    case 0:
      if(currentLiftHeight >= CAP_FLIP_MIN_HEIGHT)
        flipCapStage++;
      else
        {
          capWasOnGround = true;
          moveLift(true, false);
        }
        break;
    case 1:
      if(flipCap())
        flipCapStage++;
      break;
    case 2:
      if(!capWasOnGround || setLiftHeight(0))
      {
        flipCapStage++;
        return true;
      }
  }

  return false;
}

int flipCapDir = 0;
/**
  Flips the cap autonomously. First we check if we are closer to the min or max.
  If min, then we will be rotating to the right. If max, rotating to the left.
  This makes sure that if we are in a weird limbo in the middle, it will sort
  itself out.
*/
bool flipCap()
{
  if(flipCapDir == 0)
  {
    if(Hardware::capFlipperMotor.get_position() < (CAP_FLIP_MIN + CAP_FLIP_MAX) / 2.0)
      flipCapDir = 1;
    else
      flipCapDir = -1;
  }

  if((flipCapDir == 1 && Hardware::capFlipperMotor.get_position() >= CAP_FLIP_MAX)
    || (flipCapDir == -1 && Hardware::capFlipperMotor.get_position() <= CAP_FLIP_MIN))
    {
        Hardware::capFlipperMotor.move(0);
        flipCapDir = 0;
        return true;
    }

  Hardware::capFlipperMotor.move(127 * CAP_FLIP_SPEED * flipCapDir);

  return false;
}

/**
  Controls the manipulator using buttons. Use this instead of the individual methods
  because some methods conflict with eachother.
*/
void operatorControls(bool liftUp, bool liftDown, bool capFlip)
{
  flipCap(capFlip);

  if(!capFlip)
    moveLift(liftUp, liftDown);
}
