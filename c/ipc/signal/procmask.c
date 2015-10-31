#include "../util/simon.h"
#include <signal.h>
int main(){
	sigset_t sigset;
	int errno_save;
	errno_save=errno;	
	if(sigprocmask(0,NULL,&sigset)<0)
	{
		err_sys("sigprocmask error");
	}
	if(sigismember(&sigset,SIGINT))
		printf("SIGINT ");
	if(sigismember(&sigset,SIGUSR1))
		printf("SIGUSR1 ");
	if(sigismember(&sigset,SIGALRM))	
		printf("SIGALRM ");
	if(sigismember(&sigset,SIGQUIT))
		printf("SIGQUIT ");
	printf("\n");
	errno=errno_save;
}
