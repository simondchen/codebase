#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
/*
 *2015年 11月 29日 星期日 11:10:42 CST
 *<nicksimon109445@gmail.com>
 *copy stdin to stdout
 */
int main(int argc,char *argv[]){
    int c;
    //设置stdin为无缓冲
    if(setvbuf(stdin,NULL,_IONBF,0)<0){
        perror("setvbuf error");
        exit(-1);
    }
    while((c=getc(stdin))!=EOF){
        if(c=='\n')
            continue;
        if(putc(c,stdout)==EOF){
            perror("putc error");
            exit(-1);
        }
        sleep(3);
    }
    if(ferror(stdin)){
        perror("getc error");
        exit(-1);
    }
	return 0;
}
