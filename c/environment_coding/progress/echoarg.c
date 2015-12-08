#include <stdio.h>
/*
 *2015年 11月 18日 星期三 09:01:04 CST
 *<nicksimon109445@gmail.com>
 *echo the args delivered to me
 */
int main(int argc,char *argv[]){
    extern char **environ;
    //char **ptr;
    int i;
    for(i=0;i<argc;i++){
        printf("argv[%d]: %s\n",i,argv[i]);
    }
	return 0;
}
