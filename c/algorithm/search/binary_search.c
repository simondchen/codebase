#include <stdio.h>
#define NIL -1

int binary_search(int *A,int v,int p,int r){
	if(p==r){
		return (A[p]==v)?p:NIL;
	}
	int mid = (p+r)/2;
	if(A[mid]==v){
		return mid;
	}
	if(A[mid]>v){
		if(p==mid){
			return NIL;
		}
		return binary_search(A,v,p,mid-1);
	}
	return binary_search(A,v,mid+1,r);
}

int main(void){
	int A[11] = {10,1,3,6,8,9,9,13,16,18,18};
	int v;
	printf("input the number to search\n");
	scanf("%d",&v);	
	int i=binary_search(A,v,1,10);
	printf("%d\n",i);
	return 0;
}
