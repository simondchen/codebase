#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void clean(void *args){
	printf("cleanupup: %s\n",args);
}

void *ptr_fn1(void *args){
	//args is pointer to tid2
	int err;
	printf("thread 1 start\n");	
	printf("thread 1 will cancel thread 2\n");
	//sleep 1s for t2 to push
	sleep(1);
	err=pthread_cancel(*((pthread_t *)args));
	if(err!=0)
		printf("pthread_cancel thread 2 error\n");
	pthread_cleanup_push(clean,"thread 1 first handler");
	pthread_cleanup_push(clean,"thread 2 second handler");
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	return ((void *)1);
}

void *ptr_fn2(void *args){
	printf("thread 2 start\n");
	pthread_cleanup_push(clean,"thread 2 first handler");
	pthread_cleanup_push(clean,"thread 2 second handler");
	//sleep 2s for t1 to cancel
	sleep(2);
	/*if(args)
		pthread_exit((void *)2);*/
	//here thread 2 is canceled,so the following logic will not be executed,and the handler stack will all be poped out with all the handlers executed
	printf("if canceled,here will not be executed\n");
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_exit((void *)2);
}

int main(){
	pthread_t tid1,tid2;
	int err;
	void *tret;
	err=pthread_create(&tid2,NULL,ptr_fn2,(void *)1);
	if(err!=0)
	{
		printf("create thread 2 error\n");
		exit(0);
	}
	err=pthread_create(&tid1,NULL,ptr_fn1,(void *)&tid2);
	if(err!=0)
	{
		printf("create thread 1 error\n");
		exit(0);
	}
	err=pthread_join(tid1,&tret);
	if(err!=0)
	{
		printf("pthread join 1 error\n");
		exit(0);
	}
	printf("thread 1 exit code %d\n",(int)tret);
	err=pthread_join(tid2,&tret);
	if(err!=0)
	{
		printf("pthread join 2 error\n");
		exit(0);
	}
	printf("thread 2 eixt code %s\n",tret==PTHREAD_CANCELED?"PTHREAD_CANCELED":"-1");
	exit(0);
}
