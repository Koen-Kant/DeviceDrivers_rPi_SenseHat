#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int i2cRead(int iHandle, unsigned char ucAddr, unsigned char *buf, int iLen);
int i2cWrite(int iHandle, unsigned char ucAddr, unsigned char *buf, int iLen);

int main (int argc, const char* argv[])
{	
	//static int file_led = -1; // LED array
	static int file_hum = -1; // humidity/temp sensor
	//static int file_pres = -1; // pressure sensor
	//static int file_acc = -1; // accelerometer/gyro
	//static int file_mag = -1; // magnetometer

	int file_i2c;
	int x;
	static int H0_rH_x2, H1_rH_x2, T0_degC_x8;
	static int T1_degC_x8, H0_T0_OUT;
	static int H1_T0_OUT, T0_OUT, T1_OUT;	
	int Temp, Humid, tmp;
	unsigned char ucTemp[4];
	int rc;
	int H_T_out, T_out, T0_degC, T1_degC;
	int H0_rh, H1_rh;

	//----- OPEN THE I2C BUS -----
	char filename[32];
	sprintf(filename, "/dev/i2c-1");
	if ((file_i2c = open(filename,  O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return 1;
	}
	
	//----- GET THE HUMIDITY SLAVE
/*	if (ioctl(file_led, I2C_SLAVE, 0x46) < 0)
	{
		fprintf(stderr, "Failed to acquire bus for LED matrix\n");
	}

	file_acc = open(filename, O_RDWR);
	if (ioctl(file_acc, I2C_SLAVE, 0x6a) < 0)
	{
		fprintf(stderr, "Failed to acquire bus for accelerometer\n");
	}
	file_mag = open(filename, O_RDWR);
	if (ioctl(file_mag, I2C_SLAVE, 0x1c) < 0)
	{
		fprintf(stderr, "Failed to acquire bus for magnetometer\n");
	}*/
	file_hum = open(filename, O_RDWR);
	if (ioctl(file_hum, I2C_SLAVE, 0x5f) < 0)
	{
		fprintf(stderr, "Failed to acquire bus for Humidity sensor\n");
	}
/*	file_pres = open(filename, O_RDWR);
	if (ioctl(file_pres, I2C_SLAVE, 0x5C) < 0)
	{
		fprintf(stderr, "Failed to aquire bus for Pressure sensor\n");
	}*/
	// Prepare humidity sensor
	i2cRead(file_hum, 0x10, ucTemp, 1); // AV_CONF
	ucTemp[0] &= 0xc0;
	ucTemp[0] |= 0x1b; // avgt=16, avgh=32
	i2cWrite(file_hum, 0x10, ucTemp, 1);

	i2cRead(file_hum, 0x20+0x80, ucTemp, 3); // get CTRL_REG 1-3
	ucTemp[0] &= 0x78; // keep reserved bits
	ucTemp[0] |= 0x81; // turn on + 1Hz sample rate
	ucTemp[1] &= 0x7c; // turn off heater + boot + one shot
	i2cWrite(file_hum, 0x20+0x80, ucTemp, 3); // turn on + set sample rate

	// Get the H/T calibration values
	i2cRead(file_hum, 0x30+0x80, ucTemp, 16);
	H0_rH_x2 = ucTemp[0];
	H1_rH_x2 = ucTemp[1];
	T0_degC_x8 = ucTemp[2];
	T1_degC_x8 = ucTemp[3];
	T0_degC_x8 |= ((ucTemp[5] & 0x3) << 8); // 2 msb bits
	T1_degC_x8 |= ((ucTemp[5] & 0xc) << 6);
	H0_T0_OUT = ucTemp[6] | (ucTemp[7] << 8);
	H1_T0_OUT = ucTemp[10] | (ucTemp[11] << 8);
	T0_OUT = ucTemp[12] | (ucTemp[13] << 8);
	T1_OUT = ucTemp[14] | (ucTemp[15] << 8);
	if (H0_T0_OUT > 32767) H0_T0_OUT -= 65536; // signed
	if (H1_T0_OUT > 32767) H1_T0_OUT -= 65536;
	if (T0_OUT > 32767) T0_OUT -= 65536;
	if (T1_OUT > 32767) T1_OUT -= 65536;

	/*// prepare pressure sensor
	ucTemp[0] = 0x90; // turn on and set 1Hz update
	i2cWrite(file_pres, 0x20, ucTemp, 1);
	
	// Init magnetometer
	ucTemp[0] = 0x48; // output data rate/power mode
	ucTemp[1] = 0x00; // default scale
	ucTemp[2] = 0x00; // continuous conversion
	ucTemp[3] = 0x08; // high performance mode
	i2cWrite(file_mag, 0x20+0x80, ucTemp, 4);

	// Init accelerometer/gyroscope
	ucTemp[0] = 0x60; // 119hz accel
	i2cWrite(file_acc, 0x20, ucTemp, 1);
	ucTemp[0] = 0x38; // enable gyro on all axes
	i2cWrite(file_acc, 0x1e, ucTemp, 1);
        ucTemp[0] = 0x28; // data rate + full scale + bw selection
	// bits:        ODR_G2 | ODR_G1 | ODR_G0 | FS_G1 | FS_G0 | 0 | BW_G1 | BW_G0
	// 0x28 = 14.9hz, 500dps
        i2cWrite(file_acc, 0x10, ucTemp, 1); // gyro ctrl_reg1
	//----- WRITE THE INIT
	address = 0x10;
	read_return = write(hum_slave, &address, 1);
	printf("First read: %d\n", read_return); 
	read_return = read(hum_slave, buffer, 1);
	printf("Second read: %d\n", read_return);
	buffer[0] &= 0x78;
	buffer[0] |= 0x1B;
	tmp_buffer[0] = 0x10;
	memcpy(&tmp_buffer[1], buffer,1);
	read_return = write(hum_slave, tmp_buffer, 1);
	printf("Third read: %d\n", read_return);
	i2cRead(hum_slave, 0x10, buffer, 1);
	buffer[0] &= 0x78;
	buffer[0] |= 0x1B;
	i2cWrite(hum_slave, 0x10, buffer, 1);

	
	address = 0xA0;
	read_return = write(hum_slave, &address, 1);
	read_return = read(hum_slave,buffer, 3);
	buffer[0] &= 0x78;
	buffer[0] |= 0x81;
	buffer[1] &= 0x7c;
	tmp_buffer[0] = 0xA0;
	memcpy(&tmp_buffer[1], buffer, 3);
	read_return = write(hum_slave, buffer, 3);
	i2cRead(hum_slave, 0x30+0x80, buffer, 3);
	buffer[0] &= 0x78;
	buffer[0] |= 0xB1;
	buffer[1] &= 0x7c;
	i2cWrite(hum_slave,0x30+0x80, buffer, 3);*/


	//----- READ THE DATA
	x = 10;
	while(x>0){
		/*address = 0xA8;
		read_return = write(hum_slave, &address, 1);
		read_return = write(hum_slave, buffer, 4);
		i2cRead(hum_slave, 0x28+0x80, buffer, 4);
		HTOut = buffer[0] | (buffer[1]<<8);
		T_out = buffer[2] | (buffer[3]<<8);
		printf("read: %d, %d, %d, %d\n", buffer[0],buffer[1],buffer[2],buffer[3]);*/
		rc = i2cRead(file_hum, 0x28+0x80, ucTemp, 4);	
		if (rc == 4)
		{
			H_T_out = ucTemp[0] + (ucTemp[1] << 8);
			T_out = ucTemp[2] + (ucTemp[3] << 8);
			if (H_T_out > 32767) H_T_out -=65536;
			if (T_out > 32767) T_out -= 65536;
			T0_degC = T0_degC_x8 / 8;
			T1_degC = T1_degC_x8 / 8;
			H0_rh = H0_rH_x2 / 2;
			H1_rh = H1_rH_x2 / 2;
			tmp = (H_T_out - H0_T0_OUT) * (H1_rh - H0_rh)*10;
			Humid = tmp / (H1_T0_OUT - H0_T0_OUT) + H0_rh*10;
			tmp = (T_out - T0_OUT) * (T1_degC - T0_degC)*10;
			Temp = tmp / (T1_OUT - T0_OUT) + T0_degC*10;
			printf("Temp: %d, Hum %d\n", Temp, Humid);
	}
	printf("Not ready\n"); // not ready
		
		sleep(1);
		x--;
	}

	//----- CLEANUP

	return 0;
}

static int i2cRead(int iHandle, unsigned char ucAddr, unsigned char *buf, int iLen)
{
int rc;

	rc = write(iHandle, &ucAddr, 1);
	if (rc == 1)
	{
		rc = read(iHandle, buf, iLen);
	}
	return rc;
} /* i2cRead() */

int i2cWrite(int iHandle, unsigned char ucAddr, unsigned char *buf, int iLen)
{
unsigned char ucTemp[512];
int rc;

	if (iLen > 511 || iLen < 1 || buf == NULL)
		return -1; // invalid write

	ucTemp[0] = ucAddr; // send the register number first 
	memcpy(&ucTemp[1], buf, iLen); // followed by the data
	rc = write(iHandle, ucTemp, iLen+1);
	return rc-1;

} /* i2cWrite() */
