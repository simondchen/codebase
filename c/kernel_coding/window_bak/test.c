#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(void){
	int fd;
	if((fd=open("/dev/window",O_RDONLY,0))<0){
		perror("open file error\n");	
		return -1;
	}
	return 0;
}
