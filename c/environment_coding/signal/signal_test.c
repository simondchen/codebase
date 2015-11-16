#define _POSIX_SOURCE
#include <stdio.h>
#include <sys/signal.h>
#include <unistd.h>
/*
 *2015年 11月 11日 星期三 20:08:44 CST
 *<nicksimon109445@gmail.com>
 *test the default action of a signal
 */

void usr1(int signo){
    printf("received SIGUSR1\n");
    return;
}

void usr2(int signo){
    printf("received SIGUSR2\n");
    return;
}

void sig_quit(int signo){
    //printf("sig_quit called\n");
}

int main(int argc,char *argv[]){
    if(signal(SIGUSR1,usr1)==SIG_ERR){
        perror("signal SIGUSR1 error\n");
        return -1;
    }
    if(signal(SIGUSR2,usr2)==SIG_ERR){
        perror("signal SIGUSR2 error\n");
        return -1;
    }
    if(signal(SIGQUIT,sig_quit)==SIG_ERR){
        perror("signal SIGQUIT error\n");
        return -1;
    }
    /*if(signal(SIGINT,usr1)==SIG_ERR){
        perror("signal SIGINT error\n");
        return -1;
    }*/
    sleep(3);
    //printf("after sleeping\n");
    //sigset_t set,oset;
    //sigemptyset(&set);
    //sigemptyset(&oset);
    //sigaddset(&set,SIGINT);
    //sigprocmask(SIG_BLOCK,&set,&oset);
    //sigprocmask(SIG_UNBLOCK,&set,&oset);
    while(1){
    }
    return 0;
}
