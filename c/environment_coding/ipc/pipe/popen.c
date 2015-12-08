#define _POSIX_SOURCE
#include <error_util.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
/*
 *2015年 11月 26日 星期四 09:24:05 CST
 *<nicksimon109445@gmail.com>
 *implement popen and pclose
 */

FILE *popen(const char *command,const char *type){
    //整体思路是对的,只是要和pclose成一个整体
    int fd[2];
    FILE *fp=NULL;
    int flag=0;
    if(pipe(fd)<0){
        perror("fork error");
        return NULL;
    }
    if(strcmp("r",type)==0){
        flag=0;
    }else if(strcmp("w",type)==0){
        flag=1;
    }else{
        printf("type must be \"r\" or \"w\"\n");
        return NULL;
    }
    pid_t pid;
    pid=fork();
    if(pid<0){
        perror("fork error");
        return NULL;
    }else if(pid>0){
        //父进程
        switch(flag){
            case 0:
                //r
                close(fd[1]);
                fp=fdopen(fd[0],"r");
                //这里错误怎么和子进程同步呢???
                if(fp==NULL){
                    perror("fdopen error");
                    exit(-1);
                }
                break;
            case 1:
                //w
                close(fd[0]);
                fp=fdopen(fd[1],"w");
                break;
        }
    }else{
        //子进程
        switch(flag){
            case 0:
                //r
                close(fd[0]);
                if(dup2(fd[1],STDOUT_FILENO)<0){
                    perror("dup2 error");
                    exit(-1);
                }
                close(fd[1]);
                if(execl(command,"a",(char *)0)<0){
                    perror("execl error");
                    exit(-1);
                }
                //这里其实应该执行不到
                break;
            case 1:
                //w
                close(fd[1]);
                if(dup2(fd[0],STDIN_FILENO)<0){
                    perror("dup2 error");
                    exit(-1);
                }
                close(fd[0]);
                if(execl(command,"a",(char *)0)<0){
                    perror("execl error");
                    exit(-1);
                }
                //这里其实应该执行不到
                break;
        }
    }
    //从这里返回调用函数,并继续执行的只有父进程,因为子进程执行exec重新覆盖了正文段!!!
    return fp;
    //一般在fopen返回后,父进程会调用fread(fp),或者fwrite(fp);
}

int main(int argc,char *argv[]){
    printf("%s\n","abc\"r\"");
	return 0;
}
