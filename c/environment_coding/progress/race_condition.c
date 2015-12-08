#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal_util.h>
/*
 *2015年 11月 17日 星期二 21:53:56 CST
 *<nicksimon109445@gmail.com>
 *practice avoiding race condition
 */

static void charatatime(char *str){
    setbuf(stdout,NULL);
    char *ch;
    //for(ch=str;(c=*ch++)!=0;){
    //    putc(c,stdout);
    //}
    for(ch=str;*ch!='\0';ch++){
        putc(*ch,stdout);
    }
}

int main(int argc,char *argv[]){
    pid_t pid;
    if((pid=fork())<0){
        perror("fork error:");
        return -1;
    }else if(pid>0){
        charatatime("output from parent\n");
    }else{
        charatatime("output from child\n");
    }
    return 0;
}
