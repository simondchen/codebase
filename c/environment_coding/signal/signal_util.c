#define _POSIX_SOURCE
#include <stdio.h>
#include <signal.h>
#include <errno.h>
/*
 *2015年 11月 13日 星期五 21:44:29 CST
 *<nicksimon109445@gmail.com>
 *signal util func
 */

void pr_mask(char *str){
    sigset_t mask;
    sigemptyset(&mask);
    //获取当前进程的信号屏蔽字
    if(sigprocmask(0,NULL,&mask)<0){
        perror("sigprocmask error:");
        return -1;
    }
    printf("%s:",str);
    if(sigismember(&mask,SIGINT)) printf("SIGINT,");
    if(sigismember(&mask,SIGALRM)) printf("SIGALRM,");
    if(sigismember(&mask,SIGUSR1)) printf("SIGUSR1,");
    if(sigismember(&mask,SIGQUIT)) printf("SIGQUIT,");
    printf("\n");
    return;
}
