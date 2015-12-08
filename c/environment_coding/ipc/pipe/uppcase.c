#include <stdio.h>
#include <stdlib.h>
/*
 *2015年 11月 26日 星期四 10:43:19 CST
 *<nicksimon109445@gmail.com>
 *reverse uppercase to lowercase
 */
int main(int argc,char *argv[]){
    //从标准输入等待数据,并写到标准输出
    int c;
    while((c=getchar())!=EOF){
        if(c=='\n'){
            fflush(stdout);
            continue;
        }
        if(c<65 || c>91){
            printf("must be uppercase character\n");
            exit(-1);
        }
        c+=32;
        if(putchar(c)==EOF){
            perror("putchar error");
            exit(-1);
        }
    }
    exit(0);
}
