#include "Adafruit_L3GD20.h"

#define DATA_OUT 2
#define DATA_IN 3
#define CLOCK_OUT 4
#define CLOCK_IN 5

#define timeout 50

Adafruit_L3GD20 gyro;

void setup()
{
  gyro.begin(gyro.L3DS20_RANGE_250DPS);
  pinMode(DATA_IN, INPUT);
  pinMode(CLOCK_IN, INPUT);
  pinMode(DATA_OUT, OUTPUT);
  pinMode(DATA_OUT, OUTPUT);
}

byte zGyro = 1;
byte yGyro = 2;
byte xGyro = 3;
byte gyro_multiplier = 255.0 / (286.0 * 2.0);

void loop()
{
  //Read the register coming from the cortex
  byte inputRegister = getInputRegister();

  //If we indicated that the getInputRegister timed out, then restart the process.
  if(inputRegister == 255)
    return;

  byte data;
  switch(inputRegister)
  {
    case 1: 
      data = (gyro.data.z * gyro_multiplier) + 127;
    break;
    case 2:
      data = (gyro.data.y * gyro_multiplier) + 127;
    break;
    case 3:
      data = (gyro.data.x * gyro_multiplier) + 127;
    break;    
  }

  setOutput(data);
}

/**
 * Gets the register that the cortex is sending over the "clock in" and "data in" pins.
 * This will wait until it reads the first clock pulse and then start gathering data.
 * With each clock pulse, the data pin is read.
 * 
 * If the pulse is longer than the timeout, then it will return 255.
 */
byte getInputRegister()
{
   //Wait for the clock pin to change
  while(digitalRead(CLOCK_IN) == false);

  byte currentBit = 0;
  byte readBits[4];
  bool clockVal = false;

  long currentTime = millis();
  while(currentBit < 4)
  {
    if(millis() - currentTime > timeout)
      return 255;
      
    if(digitalRead(CLOCK_IN) != clockVal)
    {
      readBits[currentBit++] = digitalRead(DATA_IN);
      clockVal = !clockVal;
      currentTime = millis();
    }
  }

  byte converted = 0;
  for(byte b; b<4; b++)
    converted += pow(2, 3-b) * readBits[b];

  return converted;
}

void setOutput(byte val)
{
  bool clockVal = false;
  for(byte b; b<8; b++)
  {
    if(val >= pow(2, 7-b))
    {
      val -= pow(2, 7-b);
      digitalWrite(DATA_OUT, true);
    }else
    {
      digitalWrite(DATA_OUT, false);
    }
    clockVal = !clockVal;
    digitalWrite(CLOCK_OUT, clockVal);
  }
  digitalWrite(CLOCK_OUT, false);
}
