#include <stdio.h>
#include <util.h>

void count_sort(int *A,int *B,int k){
	//range 0..k
	int C[k+1];
	int len = A[0];
	int i;
	B[0] = len;
	for(i=0;i<=k;i++){
		C[i] = 0;
	}
	for(i=1;i<=len;i++){
		C[A[i]] = C[A[i]] + 1;	
	}
	for(i=1;i<=k;i++){
		C[i] = C[i] + C[i-1];
	}
	for(i=len;i>=1;i--){
		B[C[A[i]]] = C[A[i]];
		C[A[i]] = C[A[i]] - 1;
	}
}

int main(void){
	int A[11] = {10,3,6,17,9,2,6,13,6,8,2};	
	int B[11] = {0};
	count_sort(A,B,20);
	printarray(B,11);
	return 0;
}
