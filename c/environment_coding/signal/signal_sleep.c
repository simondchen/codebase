#include <stdio.h>
#include <sys/signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
/*
 *2015年 11月 12日 星期四 10:25:52 CST
 *<nicksimon109445@gmail.com>
 *block nanosleep by a signal
 */

void sig_int(int signo){
    return;
}

int main(int argc,char *argv[]){
    struct timespec req;
    struct timespec rem;
    req.tv_sec=10; 
    req.tv_nsec=0;
    signal(SIGINT,sig_int);
    while(nanosleep(&req,&rem)<0){
        if(errno==EINTR){
            //被信号中断,继续之前的睡眠
            perror("nanosleep interrupted:");
            printf("%ld,%09ld\n",rem.tv_sec,rem.tv_nsec);
            //这里会自动将信号处理函数改回默认处理函数!!!所以说signal是很受实现影响的,最好不要用它
            //错了,不会改成默认的-_-
            req=rem;
        }else{
            perror("error:");
            return -1;
        }
    }
    return 0;
}
