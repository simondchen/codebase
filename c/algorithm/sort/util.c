#include <util.h>
#include <stdio.h>

void printarray(int *A,int len){
	int i;
	for(i=0;i<len;i++){
		printf("%d ",A[i]);	
	}
	printf("\n");
}
