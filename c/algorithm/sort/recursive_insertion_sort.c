#include <stdio.h>


void recursive_insertion_sort(int *A,int r){
	if(r>1){	
		recursive_insertion_sort(A,r-1);
		int i=r-1;
		int k=A[r];
		while(i>0 && A[i]>k){
			A[i+1] = A[i];
			i--;
		}
		A[i+1] = k;
	}
}

int main(void){
	int A[11] = {10,4,9,2,0,5,8,7,1,5,6};
	recursive_insertion_sort(A,10);
	int i;
	for(i=1;i<=10;i++){
		printf("%d ",A[i]);
	}
	printf("\n");
	return 0;
}
