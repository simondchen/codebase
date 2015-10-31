#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <time.h>
/*
 *2015年 10月 24日 星期六 19:39:03 CST
 *<nicksimon109445@gmail.com>
 *fdset practice for stdin
 */
int main(int argc,char *argv[]){
	fd_set readfs;
	int ret;
	struct timeval tv;
	tv.tv_sec=5;
	tv.tv_usec=0;
	FD_ZERO(&readfs);
	FD_SET(0,&readfs);
	ret=select(1,&readfs,NULL,NULL,&tv);
	if(ret<0){
		printf("select error\n");
		return -1;
	}
	else if(ret){
		char buf[10];
		printf("data available\n");
		scanf("%s",buf);
		printf("%s\n",buf);
	}
	else{
		printf("no data is available within five seconds\n");
	}
	return 0;
}
