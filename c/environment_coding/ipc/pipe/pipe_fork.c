#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
/*
 *2015年 11月 23日 星期一 21:15:35 CST
 *<nicksimon109445@gmail.com>
 *utilize pipe to communicate between parent/child process
 */
//父进程--->子进程
int main(int argc,char *argv[]){

    int fd[2];
    if(pipe(fd)<0){
        perror("pipe error:");
        return -1;
    }
    pid_t pid;
    if((pid=fork())<0){
        perror("pipe error:");
        return -1;
    }else if(pid==0){
        //子进程
        //关闭子进程的写fd[1]
        close(fd[1]);
        char buf[1024];
        int n=read(fd[0],buf,1024);
        write(STDOUT_FILENO,buf,n);
    }else{
        //父进程
        //关闭父进程的读fd[0]
        char *buf="hello child\n";
        //将父进程的标准输出重定向到fd[1]
        write(fd[1],buf,strlen(buf)+1);
        close(fd[0]);
    }

}
