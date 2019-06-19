#include <stdio.h>
#include "oslib/iic.h"
#include "oslib/os.h"
#include "swis.h"

int main(){
    int device;
    byte buffer[2];
    char i, Line, Color, Pixel, size;
    int out, HalArgs[8], HalRes[4], call;
    os_hardware_flags fls;
    out = 14;

    call = 31;
    fls = 0;
    os_hardware_call_hal(HalArgs[0], HalArgs[1], HalArgs[2], HalArgs[3], HalArgs[4], HalArgs[5], HalArgs[6], HalArgs[7], fls,call,HalRes, HalRes+1, HalRes+2, HalRes+3);
    out = HalRes[0];
    printf("Number of IIC buses on the system is: %d\n",out);

    call = 35;
    fls = 0;
    os_hardware_call_hal(HalArgs[0], HalArgs[1], HalArgs[2], HalArgs[3], HalArgs[4], HalArgs[5], HalArgs[6], HalArgs[7], fls,call,HalRes, HalRes+1, HalRes+2, HalRes+3);
    out = HalRes[0];
    printf("HAL_IIC Device: %d\n",out);

    call = 32;
    fls = 0;
    os_hardware_call_hal(HalArgs[0], HalArgs[1], HalArgs[2], HalArgs[3], HalArgs[4], HalArgs[5], HalArgs[6], HalArgs[7], fls,call,HalRes, HalRes+1, HalRes+2, HalRes+3);
    out = HalRes[0];
    printf("HAL_IIC Type: 0x%X\n",out);

    device = 0x46;
    for(Line=0;Line<8;Line++)
    {
        for(Color=0;Color<3;Color++)
        {
            for(Pixel=0;Pixel<8;Pixel++)
            {
                buffer[0]=Line*24+Color*8+Pixel;
                buffer[1]=63;
                _swix(XIIC_Control, _INR(0,2), (device<<1)+0, buffer, 2);
            }
        }
    }
    return 0;
}
