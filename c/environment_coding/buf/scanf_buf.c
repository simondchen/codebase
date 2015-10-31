#include <stdio.h>
#include <stdlib.h>

void handler(int sig){
	
	exit(0);
}

int add(int a,int b){
	return a+b;
}

int main(void){
	int i;
	printf("-");
	printf("\n");
	add(1,2);
	while(1);
	return 0;
}
