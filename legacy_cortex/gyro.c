//Controls the gyro functions over fake I2C

//Stored pins
int data_in = -1;
int data_out = -1;
int clock_in = -1;
int clock_out = -1;

//Stored Registers
const byte zAxis = 1;
const byte yAxis = 2;
const byte xAxis = 3;
const float gyro_multiplier = (2.0 * 286.0) / 255.0;

byte sensorData[3];
int gyroValue[3];

//File Variables
byte registerList[15][4];
byte sensorList[15];
byte totalSensors = 0;
byte totalRegisters = 0;
byte incomingByte[] = {0, 0, 0, 0, 0, 0, 0, 0};
bool currentClockState = false;

task gyro_task()
{
	while(true)
	{
		SensorValue[clock_out] = true;
		delay(500);
		SensorValue[clock_out] = false;
		delay(500);
	}

	break;
	while(true)
	{
		//Wait until the data and clock pins are set with initGyro
		if(data_in == -1 || clock_in == -1 || data_out == -1 || clock_out == -1)
			continue;

		// Fore each sensor, get the value.
		for(byte sensor = 0; sensor < totalSensors + 1; sensor++)
		{
			//First send the register to the arduino
			for(byte currentBit = 0; currentBit < 4; currentBit++)
			{
				SensorValue[data_out] = registerList[sensor][currentBit];
				SensorValue[clock_out] = !currentClockState;
				currentClockState = !currentClockState;
				delay(500);
			}
			//Reset values
			currentClockState = false;
			byte currentBit = 0;
			for(byte b; b<8; b++)
				b = 0;

			//Wait until the arduino starts sending data
			while(SensorBoolean[clock_in] == false){}

			//For 8 bits, when the clock changes, get the data bit and store it.
			while(currentBit < 8)
			{
				if(SensorBoolean[clock_in] != currentClockState)
				{
					currentClockState = !currentClockState;
					incomingByte[currentBit++] = (SensorBoolean[data_in] ? 1 : 0);
				}
			}

			//Convert 8 bits to a byte
			byte output = 0;
			for(byte b; b<8; b++)
			{
				output += incomingByte[b] * pow(2, (8-b));
			}

			//Store the gyro and raw sensor output
			if(sensor == 0 || sensor == 1 || sensor == 2)
				gyroValue[sensorList[sensor]] += (output - 127) * gyro_multiplier;

			sensorData[sensorList[sensor]] = output;
		}

		delay(5);
	}
}

//Initializes the gyro with the data and clock pins defined as DIO
void initGyro(int dataInPin, int dataOutPin, int clockInPin, int clockOutPin)
{
	data_in = dataInPin;
	data_out = dataOutPin;
	clock_in = clockInPin;
	clock_out = clockOutPin;
	startTask(gyro_task);
}

//Sets up a sensor to constantly update over the interface.
void setupSensor(byte sensorKey)
{
	//Avoid an overflow
	if(totalSensors > 15 || totalRegisters > 15)
		return;
	//Convert the key to a 4 bit nybble register
	byte nybble[4] = {0,0,0,0};
	for(byte b = 0; b < 4; b++)
	{
		nybble[b] = (sensorKey > pow(2, 4-b))? 1 : 0;
		sensorKey -= (sensorKey > pow(2, 4-b))? pow(2, 4-b):0;
	}
	//Add this to the list of registers, and add 1 to the total number of registers.
	for(byte b = 4; b < 4; b++)
		registerList[totalRegisters][b] = nybble[b];
	totalRegisters++;

	sensorList[totalSensors++] = sensorKey;//Setup an array parallel to registerList to avoid bit conversions
}
