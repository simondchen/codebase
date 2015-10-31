#include "../util/simon.h"
#include <mqueue.h>

static volatile sig_atomic_t flag;
static void sig_usr1(int);

int main(int argc,char **argv){
	mqd_t mqd;
	void *buff;
	struct mq_attr attr;
	struct sigevent sev;
	sigset_t new,old,zero;
	ssize_t n;
	if(argc!=2)
		err_quit("usage:mqnotify <name>");
	if((mqd=mq_open(argv[1],O_RDONLY))<0)
		err_sys("mq_open error");
	if(mq_getattr(mqd,&attr)<0)
		err_sys("mq_getattr error");
	buff=malloc(attr.mq_msgsize);
	if(buff==NULL)
		err_sys("malloc error");
	sigemptyset(&new);
	sigemptyset(&zero);
	sigaddset(&new,SIGUSR1);
	serv.sigev_notify=SIGEV_SIGNAL;
	serv.sigev_signo=SIGUSR1;
	if(mq_notify(mqd,&sev)==-1)
		err_sys("mq_notify error first");
	//block signal 
	for(;;){
		if(sigprocmask(SIG_BLOCK,&new,&old)<0){
			err_sys("sigprocmask SIG_BLOCK error");
		}
		while(flag==0)
			sigsuspend(&zero);
		flag=0;
		if(mq_notify(mqd,&sev)<0)
			err_sys("mq_notify error second");
		if((n=mq_receive(mqd,buff,attr.mq_msgsize,NULL))<0)
			err_sys("mq_receive error");
		printf("%ld bytes received\n",(long)n);
		if(sigprocmask(SIG_SETMASK,&old,NULL)<0)
			err_sys("sigprocmask SIG_SETMASK error");
	}
	exit(0);
}

static void sig_usr1(int signo){
	flag=1;
}
