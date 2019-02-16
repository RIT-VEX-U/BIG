#ifndef EXTERNAL_FUNCTIONS
#define EXTERNAL_FUNCTIONS

#define LOW_FLAG_HEIGHT
#define MID_FLAG_HEIGHT
#define HIGH_FLAG_HEIGHT
#define IN 127
#define OUT -127

//Driving Code
void drive(int leftMotor, int rightMotor);
void driveTank(float left, float right, bool square_inputs);
void driveMecanum(int leftYJoy, int leftXJoy, int rightXJoy, bool squareInputs, float deadband);
void driveMecanumRaw(float magnitude, float direction, float rotation, bool squareInputs, float deadband);

//Manipulator Code
float getAverageEncoderValue(int liftStage);
bool setLiftHeight(float inches);
float getLiftHeight(void);
int moveLift(bool buttonUp, bool buttonDown);
bool flipCap(void);
bool flipCap(bool button);
void operatorControls(bool liftUp, bool liftDown, bool capFlip);

//
#endif
