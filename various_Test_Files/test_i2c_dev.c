#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>                             //Needed for I2C port
#include <fcntl.h>                              //Needed for I2C port
#include <sys/ioctl.h>                  //Needed for I2C port
#include <linux/i2c-dev.h>              //Needed for I2C port
#include <stdlib.h>

int main( int argc, const char* argv[] )
{
	int fd;
	char buff[200];
	int tmp,i,addr;
	char buffer[2]; 
	
	buffer[0] = 1;
	buffer[1] = 2;
	addr = strtoul(argv[1],0,16);
	fd = open("/dev/i2c-1", O_RDONLY|O_NONBLOCK);
	if(ioctl(fd,I2C_SLAVE,addr)<0)
		printf("#Fail\n");
	printf("Testing: %d\n", fd);
	while(1)
	{
		write(fd, &buffer,2);
		tmp = read(fd ,&buff,0);
		printf("Read %d bytes, which where:",tmp);
		for (i=0; i<tmp;i++)
		{
			printf("%c",buff[i]);
		}	
		printf("\n");
		sleep(1);
	}
	return 0;
}

