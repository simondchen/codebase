#define _POSIX_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
/*
 *2015年 11月 28日 星期六 09:31:11 CST
 *<nicksimon109445@gmail.com>
 *test the property of buf
 */
void sig_int(int signo){
    int i;
    scanf("%d",&i);
    printf("%d",i);
}


int main(int argc,char *argv[]){
    struct sigaction sa;
    memset(&sa,0,sizeof(sa));
    sa.sa_handler=sig_int;
    sigaction(SIGINT,&sa,NULL);
    printf("a");
    while(1){
        sleep(1);
    }
	return 0;
}
