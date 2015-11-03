#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
/*
 *2015年 10月 27日 星期二 21:42:55 CST
 *<nicksimon109445@gmail.com>
 *test
 */
int main(int argc,char *argv[]){
    int8_t a=0xb3;
    usleep(10000);
    printf("%d\n",a);
	return 0;
}
