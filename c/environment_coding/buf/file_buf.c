#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(void){
	char *buf = "-";
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	int fd = open("a",O_RDWR | O_CREAT,mode);
	if(fd<0){
		perror("open error\n");
	}
	write(fd,buf,1);
	while(1);
	return 0;
}
