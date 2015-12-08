#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
/*
 *2015年 11月 17日 星期二 18:18:17 CST
 *<nicksimon109445@gmail.com>
 *create a zombie progress
 */

int main(int argc,char *argv[]){
    pid_t pid=fork();
    if(pid<0){
        perror("fork error:");
        return -1;
    }else if(pid==0){
        printf("child exit\n");
        exit(2);
    }else{
        int status;
        sleep(3);
        wait(&status);
    }
	return 0;
}
