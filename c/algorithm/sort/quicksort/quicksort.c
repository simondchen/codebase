#include <stdio.h>

int partition(int a[],int m,int n){
	int i=m-1;
	int x = a[n];
	for(int j=m;j<n;j++){
		if(a[j]<x){
			i=i+1;
			int tmp=a[i];
			a[i]=a[j]; 
			a[j]=tmp;
		}
	}
	a[n]=a[i+1];
	a[i+1]=x;
	return i+1;
}

void quicksort(int a[],int m,int n){
	if(m<n){
		int q = partition(a,m,n);
		quicksort(a,m,q-1);
		quicksort(a,q+1,n);
	}
}

int main(){
	int a[8] = {2,8,7,1,3,5,6,4};
	quicksort(a,0,7);
	//int i = partition(a,0,7);
	//printf("%d\n",i);
	for(int i=0;i<8;i++)
		printf("%d ",a[i]);
	printf("\n");
	return 0;
}
