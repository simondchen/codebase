#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
int main(int argc,char **argv){
	mqd_t mqdes;
	int c,flags;
	int error;
	flags=O_RDWR|O_CREAT;
	while((c=getopt(argc,argv,"e"))!=-1){
		switch(c){
			case 'e':
				flags|=O_EXCL;
				break;
			default:	
				break;
		}
	}
	if(optind!=argc-1){
		printf("usage: mqcreate [-e] <name>\n");
		return -1;
	}
	if((mqdes=mq_open(argv[optind],flags,FILE_MODE,NULL))==-1){
		printf("mq_open error\n");
		printf("%s\n",strerror(error));
		return -1;
	}
	mq_close(mqdes);
	exit(0);
}
