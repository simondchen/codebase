#include <error_util.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
/*
 *2015年 11月 18日 星期三 21:53:39 CST
 *<nicksimon109445@gmail.com>
 *handle error
 */
#define MAXLINE 256
//可变参数
static void err_doit(int errnoflag,int errno,const char *fmt,va_list ap);

void err_sys(const char *message,...){
    va_list ap;
    va_start(ap,message);
    //err_doit(1,errno,message,ap);
    va_end(ap);
    exit(1);
}

static void err_doit(int errnoflag,int errno,const char *fmt,va_list ap){
    char buf[MAXLINE];
    vsnprintf(buf,MAXLINE,fmt,ap);
    if(errnoflag){
        snprintf(buf+strlen(buf),MAXLINE-strlen(buf),": %s",strerror(errno));
    }
    strcat(buf,"\n");
    fflush(stdout);
    fputs(buf,stderr);
    fflush(NULL);
}

void perr_exit(const char *msg){
    perror(msg);
    exit(-1);
}
