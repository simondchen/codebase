#include <stdio.h>

int partition(int a[],int m,int n){
	int i=m-1;	
	int j;
	int x=a[n];
	for(j=m;j<n;j++){
		if(a[j]<x){
			i++;
			int tmp;
			tmp=a[j]; 
			a[j]=a[i];
			a[i]=tmp;
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
	int a[]={3,1,5,1,3,6,2,7,};
	int size=sizeof(a)/sizeof(int);
	quicksort(a,0,size-1);
	int i;
	for(i=0;i<size;i++){
		printf("%d\n",a[i]);
	}	
	return 0;
}
