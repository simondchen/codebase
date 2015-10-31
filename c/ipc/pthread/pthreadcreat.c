#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
void printids(const char *s){
	pid_t pid;
	pthread_t tid;
	pid=getpid();
	tid=pthread_self();
	printf("%s: pid %u tid %u (0x%x)\n",s,(unsigned int)pid,(unsigned int)tid,(unsigned int)tid);
}

void *thr_fn(void *args){
	printids("new thread");
	return ((void *)0);
}

int main(){
	pthread_t tid;
	int err;
	err=pthread_create(&tid,NULL,thr_fn,NULL);
	if(err!=0){
		printf("pthread_create error:%s\n",strerror(err));
		exit(0);
	}
	printids("main thread");
	sleep(1);
	exit(0);
}
