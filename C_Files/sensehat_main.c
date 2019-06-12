#include "sensehat_C.h"
#include "sensehat_sensors.h"
#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
        int i, line;
        unsigned char MonoArray[64], redArray[64], greenArray[64], blueArray[64];
	char done =0;
        int pX, pY, tX, tY, score, scoreoverflow

	fd_hummidity_sensor = -1;   // de HTS221 op I2C port 0x5F
	fd_magnetometer_sensor = -1;// de LSM9DS1 op I2C port 0x1C
	fd_accelGyro_sensor = -1;   // de LSM9DS1 op I2C port 0x6A
	fd_LEDMatrixJoystick_sensor = -1;   // de AtTiny op I2C port 0x46
	fd_presure_sensor = -1;     // de LPS25H op I2C port 0x5C

	sprintf(i2cPort, "/dev/i2c-1");
	HumSensorHTS221SetupConfigData();
	LED_JoystickSensorSetup();
	memset(&MonoArray, 0, 64);

	srand(time(NULL));
	
	pX = 4; //left to right
	pY = 4;	//Down to upi

	tX = ((double)rand()/(double)RAND_MAX)*7;
	tY = ((double)rand()/(double)RAND_MAX)*7;
	
	score = 0;
	while(!done)
	{	
		memset(&greenArray,0,64);
		memset(&blueArray, 0, 64); 	//Set the Player
		blueArray[pX+(pY*8)]=30;					

		memset(redArray,0, 64); 	//Set the Target
		redArray[tX+(tY*8)]=63;

		LEDSetMatrixChannels(redArray, greenArray, blueArray);

		usleep(10000);
		line = (int)JoystickGetData();
		switch(line)
		{
			case 0:
				//None
				break;
			case 2: //2
				//Left
				if(pX!=7)
				{
					pX++;
				}
				break;
			case 1: //1
				//Up
				if(pY!=7)
				{
					pY++;
				}
				break;
			case 8:
				//Enter
				done = 1;
				break;
			case 16: //16
				//Right
				if(pX!=0)
				{
					pX--;
				}
				break;
			case 4: //4
				//Down
				if(pY!=0)
				{
					pY--;
				}
				break;
			default:
				break;
	
		}
		if(pX==tX && pY==tY)
		{
			score++;	
			tX = ((double)rand()/(double)RAND_MAX)*7;
			tY = ((double)rand()/(double)RAND_MAX)*7;
		}	
	}
	memset(&MonoArray,0,64);
	if(score>63)
	{
		Score=63;
	}
	for(i=0;i<score;i++)
	{
		MonoArray[i]=20;
	}
	LEDSetMonoMatrix(MonoArray,3);
	sleep(1);
	cleaUp();
	return 0;
}
