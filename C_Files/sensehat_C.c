#include "sensehat_C.h"

int i2cWrite(int i2cDevice, unsigned char deviceAddress, unsigned char *writeBuffer, int bufferLenght)
{
    unsigned char Buffer[512];
    int returnValue;

    Buffer[0] = deviceAddress;                      
    memcpy(&Buffer[1], writeBuffer, bufferLenght);  // prefix the databuffer with the write address so the device knows where to write to.
    returnValue = write(i2cDevice, Buffer, bufferLenght+1);
    return returnValue-1;
}


int i2cRead(int i2cDevice, unsigned char deviceAddress, unsigned char *writeBuffer, int bufferLenght)
{
int returnValue;
        
        returnValue = write(i2cDevice, &deviceAddress, 1);
        if (returnValue == 1)
        {       
                returnValue = read(i2cDevice, writeBuffer, bufferLenght);
        }
        return returnValue;
}

void cleaUp()
{
    unsigned char buffer[192];
    memset(&buffer,0,192);
    i2cWrite(fd_LEDMatrixJoystick_sensor, 0, buffer, 192);
    close(fd_hummidity_sensor);
    close(fd_magnetometer_sensor);
    close(fd_accelGyro_sensor);
    close(fd_LEDMatrixJoystick_sensor);
    close(fd_presure_sensor);
}
