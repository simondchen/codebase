#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
/*
 *2015年 11月 29日 星期日 11:24:48 CST
 *<nicksimon109445@gmail.com>
 *test how shell deliver input from keyboard to driver
 */
int main(int argc,char *argv[]){
    char buf[1];
    int c;
    int i=49;
    write(1,&i,1);
    sleep(2);
    while((c=read(0,buf,1))>0){
        if(buf[0]==EOF){
            exit(1);
        }
        write(1,buf,1);
    }
    if(c<0){
        perror("read error");
        exit(-1);
    }
	return 0;
}
