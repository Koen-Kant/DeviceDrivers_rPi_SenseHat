#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    char input_dev[] = "/dev/fb1";
    struct pollfd fds[1];
    int i;
    fds[0].fd = open(input_dev, O_RDWR);

    if(fds[0].fd<0)
    {
        printf("error unable open for reading '%s'\n",input_dev);
        return(0);
    }

    const int input_size = 4096;
    unsigned char input_data[input_size];
    memset(input_data,0,input_size);

    fds[0].events = POLLIN;
    
    memset(input_data,0b00000111,input_size);
	//Bit: RRRRRGGG
    for(i=0;i<128;i+=2)
    {
	input_data[i] = 0b11100000;	
	//BIT: GGGBBBBB
    }	
    int r = write(fds[0].fd,input_data,128);
    printf("Writen: %d bytes\n", r);
    close(fds[0].fd);
    return 0;
}
