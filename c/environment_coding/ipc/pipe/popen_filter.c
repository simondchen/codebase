#define _BSD_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <error_util.h>
/*
 *2015年 11月 26日 星期四 10:50:54 CST
 *<nicksimon109445@gmail.com>
 *call the lowercase program to filter
 */
int main(int argc,char *argv[]){
    //stdout是行缓冲的,所以要调用fflush及时打印到终端上
    FILE *fp;
    if((fp=popen("./uppcase","r"))==NULL){
        perror("popen error");
        return -1;
    }
    for(;;){
        fputs("<promt>:",stdout);
        fflush(stdout);
        char buf[1024];
        if(fgets(buf,1024,fp)==NULL){
            break; /*EOF*/
        }
        if(fputs(buf,stdout)==EOF){
            perror("fputs error");
            exit(-1);
        }
    }
    if(pclose(fp)==-1)
        perr_exit("pclose error");
    putchar('\n');
    exit(0);
}
