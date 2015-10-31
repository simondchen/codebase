#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
int main(int argc,char **argv){
	mqd_t mqd;
	int c,error,flags;
	uint32_t prio;
	ssize_t n;
	struct mq_attr attr;
	char *buf;
	flags|=O_RDONLY;
	while((c=getopt(argc,argv,"n"))!=-1){
		switch(c){
			case 'n':
				flags|=O_NONBLOCK;	
				break;
			default:
				break;
		}
	}
	if(optind != argc-1){
		printf("wrong parm\n");
		return -1;
	}
	if((mqd=mq_open(argv[optind],flags))==-1)
	{
		printf("mq_open error\n");
		return -1;
	}
	if((error=mq_getattr(mqd,&attr))==-1)
	{
		printf("getattr error\n");
		return -1;
	}
	buf=malloc(attr.mq_msgsize*sizeof(char));
	if(buf==NULL)
	{
		printf("malloc error\n");
		return -1;
	}
	if((n=mq_receive(mqd,buf,attr.mq_msgsize,&prio))==-1)
	{
		printf("receive error\n");
		return -1;
	}
	printf("read %ld bytes,priority = %u\n",(long)n,prio);
	return 0;
}
