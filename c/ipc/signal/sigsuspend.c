#include "../util/simon.h"

static volatile sig_atomic_t flag;

static void sig_int(int signo){
	if(signo==SIGINT)
	{
		printf("interrupt\n");
	}else if(signo==SIGQUIT)
	{
		flag=1;
	}
}

int main(){
	sigset_t new,old,zeromask;
	sigemptyset(&new);
	sigemptyset(&old);
	sigemptyset(&zeromask);
	sigaddset(&new,SIGQUIT);
	if(signal(SIGINT,sig_int)==SIG_ERR)
		err_sys("signal SIGINT error");
	if(signal(SIGQUIT,sig_int)==SIG_ERR)
		err_sys("signal SIGQUIT error");
	if(sigprocmask(SIG_BLOCK,&new,&old)==-1)
	{
		err_sys("sigprocmask error");
	}
	while(flag==0){
		sigsuspend(&zeromask);
		if(flag==0)
			printf("from sigint");
		else if(flag==1)
			printf("from sigquit");
	}
	flag=0;
	if(sigprocmask(SIG_SETMASK,&old,NULL)<0)
	{
		err_sys("sigprocmask sig_setmask error");
	}
	exit(0);
}
