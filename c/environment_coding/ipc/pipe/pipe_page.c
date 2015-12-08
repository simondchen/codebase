#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
/*
 *2015年 11月 24日 星期二 20:38:45 CST
 *<nicksimon109445@gmail.com>
 *list the output page from page
 */
int main(int argc,char *argv[]){

    FILE *file;
    if(argc!=2){
        printf("Usage:./pipe_page pathname\n");
        return -1;
    }
    if((file=fopen(argv[1],"r"))==NULL){
        perror("fopen error:");
        return -1;
    }
    //创建管道
    int fd[2];
    if(pipe(fd)<0){
        perror("pipe error:");
        return -1;
    }
    pid_t pid;
    if((pid=fork())<0){
        close(fd[0]);
        close(fd[1]);
        perror("fork error:");
        return -1;
    }else if(pid>0){
        //父进程
        close(fd[0]);
        //读文件并写到管道中去
        char buf[1024];
        int total=0;
        while(fgets(buf,1024,file)!=NULL){
            int len=strlen(buf);
            if(write(fd[1],buf,len)!=len){
                perror("write buf error:");
                return -1;
            }
            total+=len;
            printf("write %d\n",total);
        }
        perror("fgets error:");
        if(ferror(file)){
            perror("fgets error:");
            return -1;
        }
        printf("out of while\n");
		if(waitpid(pid,NULL,0)<0){
		    perror("waitpid error:");
		    return -1;
		}
        close(fd[1]);
        exit(0);
    }else{
        close(fd[1]);
        if(dup2(fd[0],STDIN_FILENO)<0){
            perror("dup2 STDIN error\n");
            close(fd[0]);
            return -1;
        }
        //获取分页程序
        char *page=NULL;
        if((page=getenv("PAGER"))==NULL){
            page="/bin/more";
        }
        char *argv0;
        if((argv0=strrchr(page,'/'))!=NULL){
            argv0++;
        }else{
            argv0=page;
        }
        //执行
        //if(execl(page,argv0,(char *)0)<0){
        if(execl(page,"a",(char *)0)<0){
            perror("exec error:");
            return -1;
        }
    }
}
