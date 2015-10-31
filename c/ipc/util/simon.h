#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#define MAXLINE 4096

static void err_doit(int,int,const char*,va_list);

void err_ret(const char *fmt, ...){
	va_list ap;
	va_start(ap,fmt);
	err_doit(1,LOG_INFO,fmt,ap);
	va_end(ap);
	return;
}

void err_sys(const char *fmt, ...){
	va_list ap;
	va_start(ap,fmt);
	err_doit(1,LOG_ERR,fmt,ap);
	va_end(ap);
	exit(1);
}

void err_dump(const char *fmt, ...){
	va_list ap;
	va_start(ap,fmt);
	err_doit(1,LOG_ERR,fmt,ap);
	va_end(ap);
	abort();
	exit(1);
}

void err_msg(const char *fmt, ...){
	va_list ap;
	va_start(ap,fmt);
	err_doit(0,LOG_INFO,fmt,ap);
	va_end(ap);
	return;
}

void err_quit(const char *fmt, ...){
	va_list ap;
	va_start(ap,fmt);
	err_doit(0,LOG_ERR,fmt,ap);
	va_end(ap);
	exit(1);
}

static void err_doit(int errnoflag,int level,const char *fmt,va_list ap){
	int errno_save,n;
	char buf[MAXLINE];
	errno_save=errno;
	vsnprintf(buf,sizeof(buf),fmt,ap);
	n=strlen(buf);
	if(errnoflag){
		snprintf(buf+n,sizeof(buf)-n,": %s",strerror(errno_save));
	}
	strcat(buf,"\n");
	fflush(stdout);
	fputs(buf,stderr);
	fflush(stderr);
	return;
}

void pr_mask(const char *str){
	sigset_t sigset;
	int errno_save;
	errno_save=errno;
	if(sigprocmask(0,NULL,&sigset)<0)
		err_sys("sigprocmask error");
	printf("%s",str);
	if(sigismember(&sigset,SIGINT)) printf("SIGINT ");
	if(sigismember(&sigset,SIGQUIT)) printf("SIGQUIT ");
	if(sigismember(&sigset,SIGUSR1)) printf("SIGUSR1");
	if(sigismember(&sigset,SIGALRM)) printf("SIGALRM ");
	printf("\n");
	errno=errno_save;
}
