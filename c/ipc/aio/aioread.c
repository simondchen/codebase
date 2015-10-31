#include "../util/simon.h"
#include <aio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUFSIZE 126
static volatile sig_atomic_t quitflag;

static void sig_quit(int);

static void sig_usr1(int,siginfo_t *,void *);

struct aio_list{
	int reqNum;
	struct aiocb *aiocb;
	int status;
};

int main(int argc,char **argv){
	int listNum,seqNum;
	struct aio_list *list;
	struct aiocb *aiolist;
	int i,s;
	struct sigaction sa;
	sigset_t new;
	if(argc<2)
		err_sys("usage:aioread <name> <name>");
	listNum=argc-1;
	//register signal handler
	sigemptyset(&new);
	sa.sa_handler=sig_quit;
	sa.sa_flags=SA_RESTART;
	sa.sa_mask=new;
	if(sigaction(SIGQUIT,&sa,NULL)<0)
		err_sys("sigaction SIGQUIT error");
	sa.sa_flags=SA_RESTART|SA_SIGINFO;
	sa.sa_sigaction=sig_usr1;
	if(sigaction(SIGUSR1,&sa,NULL)<0)
		err_sys("sigaction SIGUSR1 error");
	if((list=calloc(listNum,sizeof(struct aio_list)))==NULL)
		err_sys("calloc aio_list error");
	if((aiolist=calloc(listNum,sizeof(struct aiocb)))==NULL)
		err_sys("calloc aiolist error");
	for(i=0;i<listNum;i++)
	{
		list[i].reqNum=i;
		list[i].status=EINPROGRESS;
		/*
		if((list[i].aiocb=calloc(1,sizeof(struct aiocb)))==NULL)
			err_sys("calloc %d aiocb error");
		*/
		//this way is better
		list[i].aiocb=&aiolist[i];
		list[i].aiocb->aio_fildes=open(argv[i+1],O_RDONLY);
		printf("opened %s on descriptor %d",argv[i+1],list[i].aiocb->aio_fildes);
		if(list[i].aiocb->aio_fildes==-1)
			err_sys("open %s error",argv[i+1]);
		list[i].aiocb->aio_offset=0;
		list[i].aiocb->aio_reqprio=0;
		list[i].aiocb->aio_buf=calloc(BUFSIZE,sizeof(char));
		if(list[i].aiocb->aio_buf==NULL)
			err_sys("calloc aiobuf error");
		list[i].aiocb->aio_nbytes=BUFSIZE;
		list[i].aiocb->aio_sigevent.sigev_notify=SIGEV_SIGNAL;
		list[i].aiocb->aio_sigevent.sigev_signo=SIGUSR1;
		list[i].aiocb->aio_sigevent.sigev_value.sival_ptr=&list[i];
		s=aio_read(list[i].aiocb);
		if(s==-1)
			err_sys("aio_read error");
	}
	seqNum=listNum;
	while(seqNum>0){
		sleep(3);
		if(quitflag==1){	
			//cancel read request
			for(i=0;i<listNum;i++)
			{
				if(list[i].status==EINPROGRESS){
					s=aio_cancel(list[i].aiocb->aio_fildes,list[i].aiocb);
					switch(s){
						case AIO_CANCELED:
							printf("aio canceled\n");
							break;
						case AIO_NOTCANCELED:
							printf("aio not canceled\n");
							break;
						case AIO_ALLDONE:
							printf("aio all done\n");
							break;
						default:
							printf("aio_cancel error");
							break;
					}
				}
			}
			quitflag=0;
		}
		printf("aio_error():\n");
		for(i=0;i<listNum;i++){
			if(list[i].status==EINPROGRESS)
			{
				printf("for request %d (descriptor %d):",i,list[i].aiocb->aio_fildes);
				list[i].status=aio_error(list[i].aiocb);
				switch(list[i].status){
					case 0:
						printf("aio success\n");
						break;
					case EINPROGRESS:
						printf("in progress\n");
						break;
					case ECANCELED:
						printf("canceled\n");
						break;
					default:
						printf("aio error");
						break;
				}
				if(list[i].status!=EINPROGRESS)
					seqNum--;
			}
		}

	}
	printf("All I/O requests completed\n");
	printf("aio_return():\n");
	for(i=0;i<listNum;i++){
		size_t s;
		s=aio_return(list[i].aiocb);
		printf("for request %d(descriptor:%d):%ld\n",i,list[i].aiocb->aio_fildes,(long)s);
	}
	exit(0);
}

static void sig_quit(int signo){
	quitflag=1;	
}

static void sig_usr1(int signo,siginfo_t *info,void *context){
	write(STDOUT_FILENO,"I/O completion signal received\n",31);
}
