#include <stdio.h>
//0,4,9
void merge(int *A,int p,int q,int r){
	int left[q-p+1];
	int right[r-q];
	int i;
	for(i=p;i<=q;i++){
		left[i-p] = A[i]; 
	}
	for(i=q+1;i<=r;i++){
		right[i-q-1] = A[i];
	}
	int m=0,n=0;
	while( m<(q-p+1) && n<(r-q) ){

		(left[m]<right[n])? (A[p+m+n]=left[m++]) : (A[p+m+n]=right[n++]);
		/*if(left[m] < right[n]){
			A[p+m+n] = left[m];	
			m++;
		}else{
			A[p+m+n] = left[n];
			n++;
		}*/
	}
	//judge why terminate
	if(m == (q-p+1)){
		while( n < (r-q)){
			A[p+m+n] = right[n];
			n++;
		}
	}else{
		while( m < (q-p+1)){
			A[p+m+n] = left[m];
			m++;
		}
	}
}

void merge_sort(int *A,int p,int r){
	if(p<r){
		int q = (p+r)/2;
		merge_sort(A,p,q);
		merge_sort(A,q+1,r);
		merge(A,p,q,r);
	}
}

int main(void){
	int A[10] = {2,6,3,9,8,1,8,2,6,5};
	int i;
	merge_sort(A,0,9);
	for(i=0;i<10;i++){
		printf("%d ",A[i]);
	}
	printf("\n");
	return 0;
}
