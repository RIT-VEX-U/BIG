//Controls the gyro code.
bool getZRegister[] = {0,0,0,1};
int wavelength = 5;//milliseconds

int storedGetZTime = 0;
int getZIteration = 0;
bool getZInit = true;
bool zBitBuilder[] = {0,0,0,0,0,0,0,0};

bool lastClockBit = 0;
int currentZState = 0;

float storedZVal = 0;

void getZAxis(int dataPin, int clockPin)
{
	switch(currentZState)
	{
	case 0:
		getZIteration = 0;
		storedGetZTime = nSysTime;
		currentZState++;
	break;
	case 1:
		if(nSysTime - storedGetZTime >= wavelength)
		{
			//Send the next bit in the register
			SensorValue[dataPin] = getZRegister[getZIteration++];
			//Change the current bit of the clock pin to it's opposite
			SensorValue[clockPin] = fabs(SensorValue[clockPin] - 1);
			storedGetZTime = nSysTime;
		}

		if(getZIteration >= 4)
		{
			getZIteration = 0;
			currentZState++;
		}
	break;
	case 2:

	if(lastClockBit != SensorValue[clockPin])
	{
		lastClockBit = SensorValue[clockPin];
		zBitBuilder[getZIteration++] = SensorValue[dataPin];
	}

	if(getZIteration >= 8)
		currentZState++;

	break;
	case 3:
		byte rawZAccel = 0;
		for(int i = 0; i < 8; i++)
		{
			rawZAccel += zBitBuilder[i] * pow(2, i);
	  }

	  storedZVal += (rawZAccel / 0.4458041958041958041958041958042) -127;
	  currentZState = 0;
	break;
	}
}
