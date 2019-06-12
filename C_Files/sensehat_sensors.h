#ifndef SH_SENS_H
#define SH_SENS_H

#include "sensehat_C.h"
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>

char i2cPort[32];

void LED_JoystickSensorSetup();
void LEDSetLEDMatrix(unsigned char input[]);
void LEDSetMonoMatrix(unsigned char input[], int setColor);
void LEDSetMatrixChannels(unsigned char red[], unsigned char green[],unsigned char blue[]);
unsigned char JoystickGetData();

void MagnetSensorLSM9DS1Setup();
int MagSensorLSM9DS1Read(int *MagX, int *MagY, int *MagZ);

void Accel_GyroSensorLSM9DS1Setup();
int AccelSensorLSM9DS1Read(int *AccelX, int *AccelY, int *AccelZ);
int gyroSensorLSM9DS1Read(int *GyroX, int *GyroY, int *GyroZ);

void presSensorLPS25HSetup();
int presSensorLPS25HRead(int *Pressure, int *Temperature);

void HumSensorHTS221Setup(unsigned char AvgHumid, unsigned char AvgTemp, unsigned char CTRL_REG1, unsigned char CTRL_REG2);
void HumSensorHTS221SetupConfigData();
int HumSensorHTS221Read(int *Temperature, int *Humidity);


void MagnetSensorLSM9DS1Setup();
void Accel_GyroSensorLSM9DS1Setup();

#endif
