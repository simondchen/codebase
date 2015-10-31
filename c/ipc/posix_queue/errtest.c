#include <errno.h>
#include <string.h>
#include <stdio.h>
int main(){
	int i;
	for(i=0;i<256;i++){
		printf("errno.%02d is %s\n",i,strerror(i));
	}
	return 0;
}
