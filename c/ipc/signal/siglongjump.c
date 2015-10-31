#include "../util/simon.h"
#include <signal.h>
#include <time.h>
#include <setjmp.h>
static void sig_alrm(int);
static void sig_usr1(int);
static sigjmp_buf buf;
static volatile sig_atomic_t canjmp;
int main(){
	if(signal(SIGALRM,sig_alrm)==SIG_ERR)
		err_sys("signal alrm error");
	if(signal(SIGUSR1,sig_usr1)==SIG_ERR)
		err_sys("signal usr1 error");
	pr_mask("in main:");
	if(sigsetjmp(buf,1)!=0){
		//ret from siglongjmp
		pr_mask("back from siglongjmp:");
		exit(0);
	}
	canjmp=1;
	for(;;)
		pause();
}

static void sig_alrm(int signo){
	pr_mask("in sig_alrm:");
}

static void sig_usr1(int signo){
	time_t starttime;
	pr_mask("in sig_usr1:");
	if(canjmp==0)
		return;
	alarm(3);
	starttime=time(NULL);
	for(;;)
	{
		if((time(NULL)-starttime)>5)
			break;
	}
	pr_mask("back from sig_alrm:");
	canjmp=0;
	siglongjmp(buf,1);
}
