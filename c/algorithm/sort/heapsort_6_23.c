#include <stdio.h>

//i starts from 1
void max_heapify(int *A,int i){
	int heap_size = A[0];
	int l = 2*i;
	int r = 2*i + 1;
	if(l > heap_size || r> heap_size){
		return;
	}
	int temp;
	if(A[i] >= A[l] && A[i] >= A[r]){
		return;
	}
	else if(A[l] > A[i] && A[l] >= A[r]){
		temp = A[i];	
		A[i] = A[l];
		A[l] = temp;
		max_heapify(A,l);
		return;
	}else{
		temp = A[i];
		A[i] = A[r];
		A[r] = temp;
		max_heapify(A,r);
		return;
	}
}

void build_max_heap(int *A){
	int n = A[0];
	int i;
	for(i=n/2;i>=1;i--){
		max_heapify(A,i);
	}
}

void heap_sort(int *A){
	int i;
	build_max_heap(A);
	int len = A[0];
	int temp;
	for(i=len;i>=2;i--){
		temp=A[1];	
		A[1]=A[i];
		A[i]=temp;
		//decrease head size
		A[0] = A[0]-1;
		max_heapify(A,1);
	}
}


int main(void){
	int A[11] = {10,3,1,6,4,8,5,2,8,7,2};
	int i;
	heap_sort(A);
	for(i=1;i<11;i++){
		printf("%d ",A[i]);
	}
	printf("\n");
}
