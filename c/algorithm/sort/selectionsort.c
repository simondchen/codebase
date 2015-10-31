#include <stdio.h>
int findMax(int *A,int len){
	int i;
	int max = 0;
	int index;
	for(i=0;i<len;i++){
		if(A[i]>max){
			max = A[i];
			index = i;
		}
	}
	return index;
}

int findMin(int *A,int len){
	int i;
	int index;
	int min=100;
	for(i=0;i<len;i++){	
		if(A[i] < min){
			min = A[i];
			index = i;
		}
	}
	return index;
}

void selection_sort(int *A,int len){
	int i;
	int index;
	int temp;
	for(i=0;i<len-1;i++){
		//index = findMax(A+i,len-i);	
		index = findMin(A+i,len-i);	
		temp = A[i];
		A[i] = A[i+index];
		A[i+index] = temp;
	}
}


int main(void){
	int A[10] = {1,5,2,7,4,9,1,9,3,8};
	int i;
	selection_sort(A,10);
	for(i=0;i<10;i++){	
		printf("%d ",A[i]);
	}
	printf("\n");
}
