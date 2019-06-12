#include "sensehat_sensors.h"
// ---------- LEDs and joystick

static int H0_rH_x2, H1_rH_x2, T0_degC_x8;
static int T1_degC_x8, H0_T0_OUT;
static int H1_T0_OUT, T0_OUT, T1_OUT;


void LED_JoystickSensorSetup()
{    
    unsigned char ReadBuffer[192];
    
    fd_LEDMatrixJoystick_sensor =  open(i2cPort, O_RDWR);    
    ioctl(fd_LEDMatrixJoystick_sensor, I2C_SLAVE, 0x46);  
    
    memset(&ReadBuffer, 0, 192);
    i2cWrite(fd_LEDMatrixJoystick_sensor, 0, ReadBuffer, 192);    
}

void LEDSetLEDMatrix(unsigned char input[])
{
	i2cWrite(fd_LEDMatrixJoystick_sensor, 0, input, 192);
}

void LEDSetMatrixChannels(unsigned char red[], unsigned char green[],unsigned char blue[])
{
        unsigned char buffer[192];
        memset(&buffer,0,192);
        int index, currline, RED, GREEN, BLUE;

        for(currline=0;currline<8;currline++)
        {
                for(index=0;index<8;index++)
                {
                        RED = index+(currline*24);
                        GREEN = (index+8)+(currline*24);
                        BLUE = (index+16)+(currline*24);
                        buffer[RED] = red[index+(currline*8)];
                        buffer[GREEN] = green[index+(currline*8)];
                        buffer[BLUE] = blue[index+(currline*8)];
                }
        }
        LEDSetLEDMatrix(buffer);

}

void LEDSetMonoMatrix(unsigned char input[], int setColor)
{	
	// Color: 3:White, 0:Red, 1:Green, 2:Blue
	// input is a 64 array, Every should correlate to the pixel
	// The data is spread over 8 color strokes, so pixel 1 is 0:8:16, 
	unsigned char buffer[192];
	unsigned char display[192];
	memset(&display,0,192);
	i2cRead(fd_LEDMatrixJoystick_sensor, 0, display, 192);
	memset(&buffer,0,192);
	int index, RED, GREEN, BLUE, currline;

	for(currline=0;currline<8;currline++)
	{
		for(index=0;index<8;index++)
		{
			RED = index+(currline*24);
                	GREEN = (index+8)+(currline*24);
                	BLUE = (index+16)+(currline*24);
			if(input[index+(currline*8)]==255)
			{
				buffer[RED] = display[RED];
				buffer[GREEN] = display[GREEN];
				buffer[BLUE] = display[BLUE];			
			}
			else
			{
				if(setColor==0||setColor==3)
				{
					buffer[RED] = input[index+(currline*8)];
				}
				if(setColor==1||setColor==3)
				{
					buffer[GREEN] = input[index+(currline*8)];
				}
				if(setColor==2||setColor==3)
				{
					buffer[BLUE] = input[index+(currline*8)];
				}
			}
		}	
	}
	LEDSetLEDMatrix(buffer);
}

unsigned char JoystickGetData()
{
	unsigned char readData[1];
	i2cRead(fd_LEDMatrixJoystick_sensor, 0xF2, readData, 1);
	return readData[0];
}

// ---------- The LSM9DS1
void MagnetSensorLSM9DS1Setup()
{
    unsigned char ReadBuffer[4];
    
    fd_magnetometer_sensor =  open(i2cPort, O_RDWR);    
    ioctl(fd_magnetometer_sensor, I2C_SLAVE, 0x1C);
    
    // Init magnetometer
    ReadBuffer[0] = 0b1001000;  // output data rate/power mode
    ReadBuffer[1] = 0b0;       // default scale
    ReadBuffer[2] = 0b0;       // continuous conversion
    ReadBuffer[3] = 0b1000;     // high performance mode
    i2cWrite(fd_magnetometer_sensor, 0x20+0x80, ReadBuffer, 4);    
}

void Accel_GyroSensorLSM9DS1Setup()
{
    unsigned char ReadBuffer[4];
    
    fd_accelGyro_sensor =  open(i2cPort, O_RDWR);    
    ioctl(fd_accelGyro_sensor, I2C_SLAVE, 0x6A);
    
    ReadBuffer[0] = 0b1100000;                          // 119hz accel
    i2cWrite(fd_accelGyro_sensor, 0x20, ReadBuffer, 1);
    ReadBuffer[0] = 0b111000;                           // enable gyro on all axes
    i2cWrite(fd_accelGyro_sensor, 0x1e, ReadBuffer, 1);
    ReadBuffer[0] = 0b101000;
    i2cWrite(fd_accelGyro_sensor, 0x10, ReadBuffer, 1);
}

int MagSensorLSM9DS1Read(int *MagX, int *MagY, int *MagZ)
{    
    unsigned char ReadBuffer[6];
    if(i2cRead(fd_magnetometer_sensor, 0x28+0x80, ReadBuffer, 6)==6)
    {
            *MagX = ReadBuffer[0] + (ReadBuffer[1] << 8);
            *MagY = ReadBuffer[2] + (ReadBuffer[3] << 8);
            *MagZ = ReadBuffer[4] + (ReadBuffer[5] << 8);
            if (*MagX > 32767) *MagX -= 65536;
            if (*MagY > 32767) *MagY -= 65536;
            if (*MagZ > 32767) *MagZ -= 65536;
            return 1;
    }
    return 0;
}

int AccelSensorLSM9DS1Read(int *AccelX, int *AccelY, int *AccelZ)
{
    unsigned char ReadBuffer[6];

    if(i2cRead(fd_accelGyro_sensor, 0x28+0x80, ReadBuffer, 6)==6)
    {     
        *AccelX = ReadBuffer[0] + (ReadBuffer[1] << 8);
        *AccelY = ReadBuffer[2] + (ReadBuffer[3] << 8);
        *AccelZ = ReadBuffer[4] + (ReadBuffer[5] << 8);
        if (*AccelX > 32767) *AccelX -= 65536;
        if (*AccelY > 32767) *AccelY -= 65536;
        if (*AccelZ > 32767) *AccelZ -= 65536;
        return 1;
    }
    return 0;
}

int gyroSensorLSM9DS1Read(int *GyroX, int *GyroY, int *GyroZ)
{
    unsigned char ReadBuffer[6];
    
    if(i2cRead(fd_accelGyro_sensor, 0x18+0x80, ReadBuffer, 6) == 6)
    {
            *GyroX = ReadBuffer[0] + (ReadBuffer[1] << 8);
            *GyroY = ReadBuffer[2] + (ReadBuffer[3] << 8);
            *GyroZ = ReadBuffer[4] + (ReadBuffer[5] << 8);
            return 1;
    }
    return 0;
}

// ---------- The LPS25H
void presSensorLPS25HSetup()
{
    unsigned char ReadBuffer[1];
    
    fd_presure_sensor = open(i2cPort, O_RDWR);    
    ioctl(fd_presure_sensor, I2C_SLAVE, 0x5C);
    
    ReadBuffer[0] = 0x90; // turn on and set 1Hz update
    i2cWrite(fd_presure_sensor, 0x20, ReadBuffer, 1);
}

int presSensorLPS25HRead(int *Pressure, int *Temperature)
{
    unsigned char ReadBuffer[5];
    int Pres, Temp;
        
    if (fd_presure_sensor != -1 && Pressure != NULL)
    {       
            if(i2cRead(fd_presure_sensor, 0x28+0x80, ReadBuffer, 5) == 5)
            {       
                    Pres = ReadBuffer[0] + (ReadBuffer[1]<<8) + (ReadBuffer[2]<<16);
                    *Pressure = Pres / 4096;
                    Temp = ReadBuffer[3] + (ReadBuffer[4] << 8);
                    if (Temp > 32767) {Temp -= 65536;}
                    Temp = 425 + (Temp / 48); 
                    *Temperature = Temp;
            }
            return 1;
    }
    return 0;
}

// ---------- The HTS221
void HumSensorHTS221Setup(unsigned char AvgHumid, unsigned char AvgTemp, unsigned char CTRL_REG1, unsigned char CTRL_REG2)
{
    fd_hummidity_sensor =  open(i2cPort, O_RDWR);    
    ioctl(fd_hummidity_sensor, I2C_SLAVE, 0x5f);
    
    //voor mpogelijke invoer, zie datasheet pag 21
    unsigned char ReadBuffer[4];
    
    if(AvgHumid == -1) {AvgHumid = 0b11;}
    if(AvgTemp == -1) {AvgTemp = 0b11;}
    if(CTRL_REG1 == -1) {CTRL_REG1 = 0b10000001;}
    if(CTRL_REG2 == -1) {CTRL_REG2 = 0b01111100;}
    
    i2cRead(fd_hummidity_sensor, 0x10, ReadBuffer, 1);
    ReadBuffer[0] &= 0b11000000; //keep the reserved bits
    ReadBuffer[0] |= ((AvgTemp<<3) || AvgHumid);
    i2cWrite(fd_hummidity_sensor, 0x10, ReadBuffer, 1);
    
    i2cRead(fd_hummidity_sensor, 0x20+0x80, ReadBuffer, 3);
    ReadBuffer[0] &= 0b01111000; // keep reserved bits
    ReadBuffer[0] |= CTRL_REG1; 
    ReadBuffer[1] &= CTRL_REG2;
    i2cWrite(fd_hummidity_sensor, 0x20+0x80, ReadBuffer, 3); // turn on + set sample rate
    
    HumSensorHTS221SetupConfigData();
}

void HumSensorHTS221SetupConfigData()
{
        //Get the config data which is spesific for this sensor, see datasheet for details
	unsigned char ReadBuffer[16];
        i2cRead(fd_hummidity_sensor, 0x30+0x80, ReadBuffer, 16);
        H0_rH_x2 = ReadBuffer[0];
        H1_rH_x2 = ReadBuffer[1];
        T0_degC_x8 = ReadBuffer[2];
        T1_degC_x8 = ReadBuffer[3];
        T0_degC_x8 |= ((ReadBuffer[5] & 0x3) << 8); // 2 msb bits
        T1_degC_x8 |= ((ReadBuffer[5] & 0xc) << 6);
        H0_T0_OUT = ReadBuffer[6] | (ReadBuffer[7] << 8);
        H1_T0_OUT = ReadBuffer[10] | (ReadBuffer[11] << 8);
        T0_OUT = ReadBuffer[12] | (ReadBuffer[13] << 8);
        T1_OUT = ReadBuffer[14] | (ReadBuffer[15] << 8);
        if (H0_T0_OUT > 32767) H0_T0_OUT -= 65536; // signed
        if (H1_T0_OUT > 32767) H1_T0_OUT -= 65536;
        if (T0_OUT > 32767) T0_OUT -= 65536;
        if (T1_OUT > 32767) T1_OUT -= 65536;
}

int HumSensorHTS221Read(int *Temperature, int *Humidity)
{
    unsigned char ReadBuffer[4];
    int read_return_value;
    int HumidOut, TempOut, TempConfig1, TempConfig2;
    int HumidConfig1, HumidConfig2;
    int holdValue;
            
            read_return_value = i2cRead(fd_hummidity_sensor, 0x28+0x80, ReadBuffer, 4); //The data address + the offset imposed by the other data chunk.
            if (read_return_value == 4)
            {       
                    HumidOut = ReadBuffer[0] + (ReadBuffer[1] << 8);
                    TempOut = ReadBuffer[2] + (ReadBuffer[3] << 8);
                    if (HumidOut > 32767) HumidOut -=65536;
                    if (TempOut > 32767) TempOut -= 65536;
                    TempConfig1 = T0_degC_x8 / 8;
                    TempConfig2 = T1_degC_x8 / 8;
                    HumidConfig1 = H0_rH_x2 / 2;
                    HumidConfig2 = H1_rH_x2 / 2;
                    holdValue = (HumidOut - H0_T0_OUT) * (HumidConfig2 - HumidConfig1)*10;
                    *Humidity = holdValue / (H1_T0_OUT - H0_T0_OUT) + HumidConfig1*10;
                    holdValue = (TempOut - T0_OUT) * (TempConfig2 - TempConfig1)*10;
                    *Temperature = holdValue / (T1_OUT - T0_OUT) + TempConfig1*10;
                    return 1;
            }
            return 0; 
}
