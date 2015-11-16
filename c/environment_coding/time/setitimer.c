#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
/*
 *2015年 11月 14日 星期六 15:22:41 CST
 *<nicksimon109445@gmail.com>
 *test and practise setitimer
 */

void sig_alarm(int signo)
{
    printf("timer elapsed\n");
}

int main(int argc,char *argv[])
{
    if(signal(SIGVTALRM,sig_alarm)==SIG_ERR){
        perror("signal error\n");
        return -1;
    }
    struct itimerval new;
    memset(&new,0,sizeof(new));
    new.it_value.tv_sec=0;
    new.it_value.tv_usec=10;
    setitimer(ITIMER_VIRTUAL,&new,NULL);
    int i=0;
    while(1){
        i++;
        usleep(10);
    }
	return 0;
}
