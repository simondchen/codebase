#include <sys/time.h>
#include <stdio.h>
#include <signal.h>

void sig_alarm(int signo){
	switch(signo)
	{
		case SIGALRM:
			printf("time elasped\n");
			break;
		default:
			break;
	}
}

int main(){
	struct itimerval new,old;	
	signal(SIGALRM,sig_alarm);
	new.it_value.tv_usec=0;
	new.it_value.tv_sec=1;
	new.it_interval.tv_usec=0;
	new.it_interval.tv_sec=1;
	setitimer(ITIMER_REAL,&new,&old);
	//for(;;);
}
