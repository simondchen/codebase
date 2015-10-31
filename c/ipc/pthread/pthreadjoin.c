#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void *ptr_fn1(void *args){
	printf("thread 1 returning\n");
	return ((void *)1);
}

void *ptr_fn2(void *args){
	printf("thread exiting\n");
	pthread_exit((void *)2);
}

int main(){
	pthread_t tid1,tid2;
	int err;
	void *tret;
	err=pthread_create(&tid1,NULL,ptr_fn1,NULL);
	if(err!=0)
	{
		printf("create thread 1 error\n");
		exit(0);
	}
	err=pthread_create(&tid2,NULL,ptr_fn2,NULL);
	if(err!=0)
	{
		printf("create thread 2 error\n");
		exit(0);
	}
	err=pthread_join(tid1,&tret);
	if(err!=0)
	{
		printf("pthread_join 1 error\n");
		exit(0);
	}
	printf("thread 1 exit code %d\n",(int)tret);
	err=pthread_join(tid2,&tret);
	if(err!=0)
	{
		printf("pthread_join 2 error\n");
		exit(0);
	}
	printf("thread 2 exit code %d\n",(int)tret);
	exit(0);
}
