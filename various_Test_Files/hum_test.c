#include <unistd.h>                             //Needed for I2C port
#include <fcntl.h>                              //Needed for I2C port
#include <sys/ioctl.h>                  //Needed for I2C port
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

struct hum_data{
	unsigned char SensorType;
	char SensName[6];
	unsigned char H0;
	unsigned char H1;
	unsigned short T0;
	unsigned short T1;
	short H0_out;
	short H1_Out;
	short T0_Out;
	short T1_Out;
	short H_Out;
	short T_Out;
	unsigned char H_Valid;
	unsigned char T_Valid;
};


int main (int agrc, const char *argv[])
{
 	FILE* fileDescriptor;	
	int fd,i;
	char filePath[] = "/dev/shm/rpi-sense-emu-humidity";
	char buffer[27];
	char *shared;

	printf("Going in\n");
	
	printf("%s couldn't be reached\n", filePath);
	fileDescriptor = fopen(filePath, "w");
	printf("We got this now: %d\n", (int)fileDescriptor);
	memset(buffer,0,27);
	printf("Have written: %d\n", fwrite(buffer, 1, 27, fileDescriptor));	
	fd = open(filePath,  O_RDWR, 777);
	ftruncate(fd, 27);
	shared = (char *)mmap(NULL, 27, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	printf("Test: %d\n",(int)shared);
	shared[0] = 1;
	shared[1] = 'B';
	shared[2] = 'y';
	shared[3] = 't';
	 
	sleep(1);
	
	while (1)
	{		
		printf("Retrieved data:\n");
		for(i=0; i<6; i++)
		{
			printf("%c", shared[i+1]);
		}
		printf("\n"); 	
		sleep(1);
	}
	unlink(filePath);
	munmap((void*)shared, 27);
	close(fd);
	fclose(fileDescriptor);
	//remove(filePath);
	return 0;
}
