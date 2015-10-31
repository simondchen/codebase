#include <stdio.h>

int partition(int *A,int p,int r){
	int radix = A[r];
	int i = p;
	int j;
	int temp;
	for(j=p;j<r;j++){
		if(A[j]<radix){	
			temp = A[j];
			A[j] = A[i];
			A[i] = temp;
			i++;
		}
	}
	temp = A[i];
	A[i] = A[r];
	A[r] = temp;
	return i;
}
	
int select(int *A,int p,int r,int i){
	if(r == p){
		return A[p];
	}
	int q = partition(A,p,r);
	int k = q-p+1;
	if(i == k){
		return A[q];
	}
	if(i<q){
		return select(A,p,q-1,i);
	}else{
		return select(A,q+1,r,i-k);
	}
}

int main(){
	int A[11] = {0,2,9,2,0,8,2,4,9,3,7};
	printf("%d\n",select(A,1,10,8));
	return 0;	
}
