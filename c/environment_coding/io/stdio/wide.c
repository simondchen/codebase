#include <stdio.h>
#include <wchar.h>
#include <errno.h>
/*
 *2015年 11月 28日 星期六 08:55:05 CST
 *<nicksimon109445@gmail.com>
 *test wide
 */
int main(int argc,char *argv[]){
    char a=0xfa;
    unsigned int i=a;
    printf("%d\n",a);
    printf("%d\n",i);
    unsigned b=0xfa;
    int j=b;
    printf("%d\n",b);
    printf("%d\n",j);
    long int p=0xffffffff;
    printf("%ld\n",p);
    short s=0xffff;
    printf("%d\n",s);
    FILE *fp=fopen("./a","w");
    if(fp==NULL){
        perror("fopen error\n");
        return -1;
    }
    fputs("hello world\n",fp);
    int wide=fwide(fp,0);
    printf("%d\n",wide);
	return 0;
}
