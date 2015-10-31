#include <stdio.h>
#include <util.h>

void count_sort(int *A,int range){
	int len = A[0];
	int B[range+1];
	int i;
	for(i=0;i<=range;i++){	
		B[i] = 0;
	}
	for(i=1;i<=len;i++){
		B[A[i]] = B[A[i]] +1;
	}
	for(i=range;i>=1;i--){
		while(B[i]--!=0){	
			A[len--] = i;	
		}
	}
}

int main(void){
	int A[11] = {10,3,5,1,9,17,8,16,5,2,1};
	int i;
	count_sort(A,20);
	printarray(A,11);
}
