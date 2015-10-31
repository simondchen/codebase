#include "../util/simon.h"
#include <signal.h>
Sigfunc *m_signal(int signo,Sigfunc *func){
	struct sigaction act,oact;
	act.sa_handler=func;
	sigemptyset(&act.sa_mask);
	if(signo==SIGALRM)
	{
		#ifdef SA_INTERRUPT
			act.sa_flags|=SA_INTERRUPT;
		#endif
	}else{
		#ifdef SA_RESTART
			act.sa_flags|=SA_RESTART;
		#endif
	}
	if(sigaction(signo,&act,&oact)<0)
	{
		return SIG_ERR;
	}
	return oact.sa_handler;
}
