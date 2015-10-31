#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(void){
	int fd;
	char buf[1024] = "helloworld\n";
	if((fd=open("/dev/my_chr",O_RDONLY,0))<0){
		perror("open");
		return -1;
	}
	//write(fd,buf,strlen("helloworld\n")+1);
	return 0;
}
