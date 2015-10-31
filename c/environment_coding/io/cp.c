#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>

int main(int argc,char *argv[]){
	char buf[2];
	int from,to;
	int len;
	mode_t mode;
	if(argc!=3){	
		printf("usage:./cp from to");
		return -1;
	}
	//设置文件mask,八进制
	mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;	
	//umask(S_IWGRP | S_IROTH | S_IWOTH);
	umask(0026);
	if((from=open(argv[1],O_RDONLY))<0){
		printf("open file for read error\n");
		return -1;
	}
	if((to=open(argv[2],O_RDWR | O_CREAT, mode))<0){
		printf("open file for write error\n");
		return -1;
	}
	/*
	while((len=read(from,buf,2))>0){
		write(to,buf,len);
	}
	*/
	/*使用sendfile*/
	while((len=sendfile(to,from,NULL,2))>0);
	
	close(from);
 	close(to);
	return 0;
}
