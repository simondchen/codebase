#include <stdio.h>
void binary_add(int *A,int *B,int *C,int len){
	int carry=0;
	int i;
	for(i=0;i<len;i++){
		C[i] = (A[i] + B[i] + carry) % 2;
		carry = (A[i] + B[i] + carry) / 2;
	}
	C[i] = carry;
}

int main(void){
	int A[10] = {1,0,1,0,1,0,1,1,0,1};
	int B[10] = {1,1,1,0,0,1,0,1,0,0};
	int C[11] = {0};
	int i;
	binary_add(A,B,C,10);
	for(i=0;i<11;i++){
		printf("%d ",C[i]);
	}
	printf("\n");
}
