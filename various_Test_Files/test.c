#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main( int argc, const char* argv[] )
{
	int fd;
	char buff[200];
	int tmp,i;
	fd = open("/dev/input/event0", O_RDONLY|O_NONBLOCK);
	printf("Testing: %d\n", fd);
	while(1)
	{
		tmp = read(fd ,&buff,0);
		printf("Read %d bytes, which where:",tmp);
		for (i=0; i<tmp;i++)
		{
			printf("%c",buff[i]);
		}	
		printf("\n");
	}
	return 0;
}

