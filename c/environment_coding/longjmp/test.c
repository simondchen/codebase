#include <stdio.h>
#include <sys/types.h>
#include <setjmp.h>
/*
 *2015年 11月 03日 星期二 11:21:00 CST
 *<nicksimon109445@gmail.com>
 *
 */

typedef int buf_t[1];

int main(int argc,char *argv[]){
    buf_t buf[2];
    jmp_buf jbuf[2];
    printf("%ld\n",sizeof(buf));
    int *a=(int *)&buf[1];
    int *b=(int *)buf;
    printf("%ld\n",(int8_t *)a-(int8_t *)b);
    printf("%ld\n",(int8_t *)&jbuf[1]-(int8_t *)jbuf);
    printf("%ld\n",sizeof(jbuf[0]));
	return 0;
}
