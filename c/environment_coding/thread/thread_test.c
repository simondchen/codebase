#include <stdio.h>
#include <pthread.h>
#include <errno.h>
/*
 *2015年 10月 24日 星期六 16:53:32 CST
 *<nicksimon109445@gmail.com>
 *Linux thread practice
 */
int i=1;

void *thread_handle(void *msg){
	printf("in thread:%s,%d\n",(char *)msg,i);
	pthread_exit(NULL);
}

int main(int argc,char *argv[]){
	pthread_t tid;
	const char *msg="thread with params";
	if(pthread_create(&tid,NULL,thread_handle,(void *)msg)<0){
		perror("pthread_create error\n");
		return -1;
	}
	pthread_exit(NULL);
}
