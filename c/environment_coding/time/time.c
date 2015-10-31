#include <stdio.h>
#include <linux/time.h>

int main(void){
	struct timeval time;
	jiffies_to_timeval(1000,&time);
	printf("%d:%d\n",time.tv_sec,time.tv_usec);
	return 0;
}
