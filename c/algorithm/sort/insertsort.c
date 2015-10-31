#include <stdio.h>
insert_sort(int *A,int len){
	int i,j,a;
	for(i=1;i<len;i++){
		a = A[i];
		for(j=0;j<i;j++){
			if(a<A[j]){
				while(i>j){
					A[i]=A[i-1];	
					i--;
				}
				A[j]=a;
				break;
			}
		}
	}
}

insert_sort2(int *A,int len){
	int i,j,key;
	for(i=1;i<len;i++){
		key = A[i];	
		//insert a into i length sorted array
		j = i-1;
		while(j>=0 && A[j]>key){	
			A[j+1] = A[j];
			j = j-1;
		}
		A[j+1] = key;
	}
}


int main(void){
	int A[10]={3,1,5,9,2,7,6,8,0,8};
	int i;
	insert_sort2(A,10);
	for(i=0;i<10;i++){
		printf("%d ",A[i]);
	}
	printf("\n");
}
