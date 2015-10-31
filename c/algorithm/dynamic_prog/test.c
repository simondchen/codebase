#include <stdio.h>
#include <stdlib.h>
int main(void){
	int *a=malloc(4*sizeof(int));
	int n;
	int (*b)[n]=(int (*)[n])a;
	int c[n];
	b[1][1]=1;
	printf("%d\n",b[1][1]);
	printf("%ld\n",sizeof(c));
	return 0;
}
