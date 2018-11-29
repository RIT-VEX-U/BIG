//Controls the gyro functions over fake I2C

//Stored pins
int data_in = -1;
int data_out = -1;
int clock_in = -1;
int clock_out = -1;

//Stored Registers
const byte zAxis = 1;
const byte zAxisRegister[4] = {0,0,0,1};
const byte yAxis = 2;
const byte xAxis = 3;
const float gyro_multiplier = (2.0 * 286.0) / 255.0;

byte sensorData[3];
int gyroValue[3];

//File Variables
const byte numOfSensors = 1;
const byte registerList[numOfSensors][4] = {{0,0,0,1}};
const byte sensorList[numOfSensors] = {1};
byte totalSensors = 0;
byte totalRegisters = 0;
byte incomingByte[] = {0, 0, 0, 0, 0, 0, 0, 0};
bool currentClockState = false;

task gyro_task()
{
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
