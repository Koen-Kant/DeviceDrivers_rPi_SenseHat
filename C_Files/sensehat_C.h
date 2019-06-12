#ifndef SH_H
#define SH_H

#include <string.h>
#include <unistd.h>

int fd_hummidity_sensor;   // de HTS221 op I2C port 0x5F
int fd_magnetometer_sensor;// de LSM9DS1 op I2C port 0x1C
int fd_accelGyro_sensor;   // de LSM9DS1 op I2C port 0x6A
int fd_LEDMatrixJoystick_sensor;   // de AtTiny op I2C port 0x46
int fd_presure_sensor;     // de LPS25H op I2C port 0x5C

// Deze code leent vrij veel van het sense-hat unchained github project, met enkele aanpassingen voor leesbaarheid.

int i2cWrite(int i2cDevice, unsigned char deviceAddress, unsigned char *writeBuffer, int bufferLenght);
int i2cRead(int i2cDevice, unsigned char deviceAddress, unsigned char *writeBuffer, int bufferLenght);
void cleaUp();

#endif
