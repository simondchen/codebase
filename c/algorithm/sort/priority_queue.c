#include <stdio.h>
#define PARENT(i) (i/2)
#define LEFT(i) (2*i)
#define RIGHT(i) (2*i+1)

void max_heapify(int *A,int i){
	int l = LEFT(i);	
	int r = RIGHT(i);
	int heap_size = A[0];
	int largest;
	if( l<=heap_size && A[l]>A[i]){
		largest = l;
	}else{
		largest = i;
	}
	if( r<=heap_size && A[r]>A[largest]){
		largest = r;
	}
	if(largest != i){
		int temp = A[largest];
		A[largest] = A[i];
		A[i] = temp;
		max_heapify(A,largest);
	}
}

void build_max_heap(int *A){
	int i;
	int len = A[0];
	for(i=len/2;i>=1;i--){
		max_heapify(A,i);
	}
}

int heap_maximum(int *A){
	return A[1];
}

int heap_extract_maximum(int *A){
	int heap_size = A[0];
	if(heap_size < 1){
		printf("heap underflow\n");
		return -1;
	}
	int max = A[1];
	int temp = A[1];
	A[1] = A[heap_size];
	A[heap_size] = temp;
	A[0] = heap_size - 1;
	max_heapify(A,1);
	return max;
}

void heap_increase_key(int *A,int i,int key){
	if(key < A[i]){
		printf("new key is smaller than current key\n");
		return;
	}
	A[i] = key;	
	while(i>1 && A[PARENT(i)]<A[i]){
		int temp = A[i];
		A[i] = A[PARENT(i)];	
		A[PARENT(i)] = temp;
		i = PARENT(i);
	}
}

void heap_insert(int *A,int key){
	A[0] = A[0] + 1;
	int heap_size = A[0];
	//minus infinite
	A[heap_size] = -100;
	heap_increase_key(A,heap_size,key);
}

void printarray(int *A){
	int i;
	int len = A[0];
	for(i=1;i<=len;i++){
		printf("%d ",A[i]);
	}
	printf("\n");
}

int main(void){
	int A[6]={5,3,9,2,8,1};
	build_max_heap(A);
	printarray(A);	
	heap_extract_maximum(A);
	printarray(A);	
	heap_insert(A,12);
	printarray(A);	
	return 0;
}
