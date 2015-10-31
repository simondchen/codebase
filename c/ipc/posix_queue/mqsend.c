#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main(int argc,char **argv){
	mqd_t mqd;
	size_t len;
	void *ptr;
	uint32_t prior;
	//unsigned int prior;
	if(argc!=4)
	{
		printf("wrong params\n");
		return -1;
	}
	if((mqd=mq_open(argv[1],O_WRONLY))==-1)
	{
		printf("mq_open error\n");
		return -1;
	}
	len=atoi(argv[2]);
	prior=atoi(argv[3]);
	if((ptr=calloc(len,sizeof(char)))==NULL){
		printf("calloc error\n");
		return -1;
	}
	if(mq_send(mqd,ptr,len,prior)==-1)
	{
		printf("mq_send error\n");
		return -1;
	}
	return 0;
}
