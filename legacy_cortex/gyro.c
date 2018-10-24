//Controls the gyro functions over fake I2C
int data = -1;
int clock = -1;
//The Z-axis register, "1", in binary
byte getZRegister[] = {0,0,0,1};

//Should be as small as possible while still functioning.
int wavelength = 5;//milliseconds

float gyroZVal = 0;

task gyro_task()
{
	while(true)
	{
		//Wait until the data and clock pins are set with initGyro
		if(data == -1 || clock == -1)
			break;

			//Send the register over in a 4-bit nybble
			for(int i = 0; i < 4; i++)
			{
				SensorValue[data] = getZRegister[i];
				SensorValue[clock] = fabs(SensorValue[clock] - 1);
				delay(wavelength);
			}


			int prevSensorVal = SensorValue[clock];
			int iteration = 0;
			byte bitBuilder[] = {0, 0, 0, 0, 0, 0, 0, 0};
			//Get the response in an 8-bit byte
			do
			{
				//Wait until the clock changes, then read the data pin. Only read until the 8th bit.
				if(prevSensorVal != SensorValue[clock])
				{
					prevSensorVal = SensorValue[clock];
					bitBuilder[iteration++] = SensorValue[data];
			  }
			}while(iteration < 8);

			//Turn the bits into decimal form
			byte rawZAccel = 0;
			for(int i = 0; i < 8; i++)
				rawZAccel += bitBuilder[i] * pow(2, i);
			//Turn the 0-255 number into degrees, and add it to the gyro value.
	  	gyroZVal += (rawZAccel / 0.4458041958041958041958041958042) -127;
	}

}

//Initializes the gyro with the data and clock pins defined as DIO
void initGyro(int dataPin, int clockPin)
{
	data = dataPin;
	clock = clockPin;
	startTask(gyro_task);
}
