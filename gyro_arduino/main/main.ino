#include "Adafruit_L3GD20.h"

Adafruit_L3GD20 gyro;
void setup() {
  if(!gyro.begin(gyro.L3DS20_RANGE_250DPS))
  {
  }
  pinMode(1, OUTPUT);
  //gyro.begin(gyro.L3DS20_RANGE_500DPS)
  //gyro.begin(gyro.L3DS20_RANGE_2000DPS)
  
}

int currentZVal = 0;
int highestVal = 0;
int lowestVal = 0;

long lastTime = 0;

void loop() {
  gyro.read();
  //int x_val = (int)gyro.data.x;
  //int y_val = (int)gyro.data.y;
  int z_val = (int)gyro.data.z;

  currentZVal -= z_val;

  if(z_val > highestVal)
    highestVal = z_val;
  else if(z_val < lowestVal)
    lowestVal = z_val;

  //Serial.println((int) (highestVal * 0.4458041958041958041958041958042));
  //Serial.println((int) (lowestVal * 0.4458041958041958041958041958042));

  // z_val right now gives between -286 and 286, and so this dumb number is 255/(2*286).
  int outputVal = (int) (z_val * 0.4458041958041958041958041958042) + 127;

  analogWrite(1, outputVal);
  
  //Serial.println(currentZVal / 125.0);
}
