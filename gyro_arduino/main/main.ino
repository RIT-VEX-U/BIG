#include "Adafruit_L3GD20.h"

#define DATA_OUT 3
#define DATA_IN 
#define CLOCK 4
#define WAVELENGTH 10

Adafruit_L3GD20 gyro;

void setup()
{
  gyro.begin(gyro.L3DS20_RANGE_250DPS);
  pinMode(DATA, INPUT);
  pinMode(CLOCK, INPUT);
}

byte zRegister = 1;

//State 1 variables: reading the incoming register
byte clockState = 0;
byte bitBuilder[] = {0, 0, 0, 0};
byte bitIteration = 0;
byte currentState = 0;

void loop()
{
  switch(currentState)
  {
    case 0:
      //When the state of the clock changes, then read the data pin.
      if(digitalRead(CLOCK) != clockState)
        {
          clockState = digitalRead(CLOCK);
          bitBuilder[bitIteration++] = digitalRead(DATA);
        }
        
        //Once we have read 4 bits, then convert it to decimal and compare to the stored registers.
        //If it equals a register, move on to the next state. If not, then reset and continue reading for clock changes.
        if(bitIteration >= 4)
        {
          byte nybbleTest = 0;
          for(byte i = 0; i < 4; i++)
            nybbleTest += bitBuilder[i] * pow(2, i);

          if(nybbleTest == zRegister)
            currentState++;
          else
            bitIteration = 0;
        }
    break;
    case 1:
      byte outputVal = (byte) (((int)gyro.data.z) * 0.4458041958041958041958041958042) + 127;
      byte bitBuilder[] = {0, 0, 0, 0, 0, 0, 0, 0};
      for(byte i = 7; i >= 0; i--)
      {
        if(outputVal >= pow(2, i))
        {
          outputVal -= pow(2, i);
          bitBuilder[i] = 1;
        }
      }

      pinMode(DATA, OUTPUT);
      pinMode(CLOCK, OUTPUT);
      byte clockState = 0;

      for(byte i = 0; i < 8; i++)
      {
        digitalWrite(CLOCK, abs(--clockState));
        digitalWrite(DATA, bitBuilder[i]);
        delay(WAVELENGTH);
      }

      currentState++;
    break;
    case 2:
      for(byte i = 0; i < 4; i++)
        bitBuilder[i] = 0;
      bitIteration = 0;

    pinMode(CLOCK, INPUT);
    pinMode(DATA, INPUT);

    currentState++;
    break;   
  }
}
