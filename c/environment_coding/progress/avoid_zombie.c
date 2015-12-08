#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
/*
 *2015年 11月 17日 星期二 19:58:25 CST
 *<nicksimon109445@gmail.com>
 *fork twice avoid creating zombie
 */
int main(int argc,char *argv[]){
    pid_t pid;
    int status;
    if((pid=fork())<0){
        perror("fork error:");
        return -1;
    }else if(pid==0){ 
        //第一个子进程,产生一个子进程,然后退出,是子进程托管给init进程
        if((pid=fork())<0){
            perror("fork error:");
            return -1;
        }else if(pid==0){
            //休眠,让第一个子进程先执行
            sleep(1);
            printf("second child,parent id=%d\n",getppid());
            exit(1);
        }else{
            return 0;
        }
    }
    //第一个子进程立即返回,所以在这里调用waitpid可以立即返回
    wait(&status);
    exit(0);
}
