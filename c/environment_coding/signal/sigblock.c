//这里一定要注意,如果定义了这个宏的话,signal在收到一次信号后会将操作重置成默认!!!
//#define _POSIX_SOURCE
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
/*
 *2015年 11月 13日 星期五 22:01:53 CST
 *<nicksimon109445@gmail.com>
 *practise procmask and sigpending
 */

void sig_quit(int signo){
    printf("in sig_quit\n");
}

int main(int argc,char *argv[]){

    sigset_t set,oset;
    sigemptyset(&set);
    sigemptyset(&oset);
    if(signal(SIGQUIT,sig_quit)==SIG_ERR){
        perror("signal error:");
        return -1;
    }
    //阻塞SIGQUIT信号
    if(sigaddset(&set,SIGQUIT)<0){
        perror("sigaddset error:");
        return -1;
    }
    if(sigprocmask(SIG_BLOCK,&set,&oset)<0){
        perror("sigprocmask error:");
        return -1;
    }
    //休眠5秒
    sleep(5);
    sigset_t pending;
    sigemptyset(&pending);
    if(sigpending(&pending)<0){
        perror("sigpending error:");
        return -1;
    }
    if(sigismember(&pending,SIGQUIT)) printf("SIGQUIT pending\n");
    //恢复之前的信号屏蔽字
    if(sigprocmask(SIG_SETMASK,&oset,NULL)<0){
        perror("sigprocmask2 error:");
        return -1;
    }
    int ret;
    //signal(SIGQUIT,sig_quit);
    if((ret=sleep(5))>0){
        if(errno==EINTR){
            perror("sleep interrupted:");
        }
        else{
            perror("sleep error:");
            return -1;
        }
    }
    printf("sleep left:%d\n",ret);
	return 0;
}
