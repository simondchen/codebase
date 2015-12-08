#define _BSD_SOURCE
#include <error_util.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
 *2015年 11月 24日 星期二 22:39:00 CST
 *<nicksimon109445@gmail.com>
 *implement page using popen
 */
int main(int argc,char *argv[]){

    if(argc!=2){
        printf("Usage: ./popen_page pathname\n");
        return -1;
    }

    FILE *fp=fopen(argv[1],"r");
    if(fp==NULL)
        perr_exit("fopen file error");

    FILE *pp=popen("/bin/more","w");
    if(pp==NULL)
        perr_exit("popen error");

    char buf[1024];
    while(fgets(buf,1024,fp)!=NULL){
        int size=strlen(buf);
        if(fputs(buf,pp)==EOF)
            perr_exit("fputs error");
    }
    if(ferror(fp))
        perr_exit("fgets error");
    if(pclose(pp)<0)
        perr_exit("pclose error");
    return 0;
}
