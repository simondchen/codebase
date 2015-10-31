#include <stdio.h>
#include <unistd.h>
/*
 *2015年 10月 25日 星期日 10:14:10 CST
 *<nicksimon109445@gmail.com>
 *test while
 */

int fun(int i){
	return i;
}

int main(int argc,char *argv[]){
	int ret;	
	int a = (ret=fun(-1));
	printf("a:%d\n",a);
	if(a){
		printf("yes\n");
	}
	/*while((ret=fun(-1))){
		printf("ret:%d\n",ret);
		sleep(2);
	}*/
	return 0;
}
