#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
/*
 *2015年 11月 26日 星期四 10:37:23 CST
 *<nicksimon109445@gmail.com>
 *test scanf of multi procress
 */
int main(int argc,char *argv[]){
    pid_t pid;
    pid=fork();
    if(pid==0){
        while(1){
            int i;
            scanf("%d",&i);
            printf("from %d:%d\n",getpid(),i);
            sleep(2);
        }
    }
    pid=fork();
    if(pid==0){
        while(1){
            int i;
            scanf("%d",&i);
            printf("from %d:%d\n",getpid(),i);
            sleep(2);
        }
    }
    while(1){
        printf("parent called\n");
        sleep(2);
    }
	return 0;
}
