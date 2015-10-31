#include "../util/simon.h"
#include <signal.h>
#include <unistd.h>
	
static void sig_quit(int);

int main(){
	sigset_t newmask,oldmask,penmask;
	if(signal(SIGQUIT,sig_quit)==SIG_ERR)
	{
		err_sys("signal error");
	}
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGQUIT);
	//block SIG_QUIT
	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
	{
		err_sys("sigprocmask error");
	}
	sleep(5);
	if(sigpending(&penmask)<0)
	{
		err_sys("sigpending error");
	}
	if(sigismember(&penmask,SIGQUIT))
		printf("\nSIGQUIT pending\n");
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
		err_sys("SIG_SETMASK error");
	printf("SIGQUIT unblocked\n");
	sleep(5);
	exit(0);
}

static void sig_quit(int signo){
	printf("caught SIGQUIT\n");
	if(signal(SIGQUIT,SIG_DFL)==SIG_ERR)
		err_sys("can't reset SIGQUIT");
}
