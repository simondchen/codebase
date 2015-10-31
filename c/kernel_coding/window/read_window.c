#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <linux/sched.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

const char *space = "|    ";
const char *line = "|___ ";

char *construct_string(int deep){
	char *str = malloc((5*deep+1)*sizeof(char));
	int i;
	if(str == NULL){
		printf("alloc str error\n");
		return NULL;
	}
	//foget this!!!
	if(deep == 0){
		str[0] = '\0';
	}
	else{
		//deep-1 not deep-i you can copy it error,how careless are you!!!
		for(i=0;i<deep-1;i++){
			strncat(str+i*5,space,5*sizeof(char));
		}
		strcat(str+i*5,line);
	}
	return str;
}

struct task_info{
	pid_t pid;	
	unsigned char order;	
	unsigned char deep;
	struct task_info *next;
};

int main(void){
	int fd;
	struct task_info buf;
	int len;
	char *str;
	if((fd=open("/dev/window",O_RDONLY,0))<0){
		perror("open file error\n");	
		return -1;
	}
	while((len=read(fd,&buf,sizeof(struct task_info)))>0){
		//list it with tree format
		str = construct_string(buf.deep);			
		if(str == NULL){
			printf("alloc str error\n");
			goto close;
		}
		printf("%s%d\n",str,buf.pid);
		//printf("%d:%d\n",buf.deep,buf.pid);
	}
	if(len<0){
		printf("read error\n");
		return -1;
	}
close:
	close(fd);
	return 0;
}
