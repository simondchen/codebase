#include <stdio.h>

void quicksort(int *A,int p,int r){
	if(r>p){
		int q = partition(A,p,r);
		quicksort(A,p,q-1);
		quicksort(A,q+1,r);
	}
}

int partition(int *A,int p,int r){
	int radix = A[r];
	int i = p;
	int j;
	int temp;
	for(j=p;j<r;j++){
		if(A[j] < radix){
			temp = A[i];
			A[i] = A[j];
			A[j] = temp;
			i++;
		}
	}
	temp = A[i];
	A[i] = A[r];
	A[r] = temp;
	return i;
}

int main(void){
	int A[10] = {2,9,1,3,8,7,4,6,5,7};
	quicksort(A,0,9);
	int i;
	for(i=0;i<10;i++){
		printf("%d ",A[i]);
	}
	printf("\n");
	return 0;
}
