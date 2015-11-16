#include <stdio.h>
/*
 *2015年 11月 12日 星期四 09:51:22 CST
 *<nicksimon109445@gmail.com>
 *test
 */
struct time{
    int a;
    int b;
};

struct time sub(struct time a,struct time b){
    struct time c;
    c.a=a.a-b.a;
    c.b=a.b-b.b;
    return c;
}
int main(int argc,char *argv[]){
    struct time a,b,c;
    int array[10],array2[10];
    a.a=3;
    a.b=2;
    b.a=2;
    b.b=1;
    c.a=0;
    c.b=0;
    c=sub(a,b);
    printf("%d:%d\n",c.a,c.b);
    c=a;
    printf("%d:%d\n",c.a,c.b);
	return 0;
}
