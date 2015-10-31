#include <stdio.h>

int partition(int a[],int m,int n){
	int i=m;
	int j=n;
	int x=a[m];
	int left=1,right=0;
	do{
		if(left){		
			if(a[j]<x){
				a[i]=a[j];
				left=0;
				right=1;
				i++;
			}else
				j--;
		}
		if(right){
			if(a[i]>x){
				a[j]=a[i];
				left=1;
				right=0;
				j--;
			}else
				i++;
		}
	}while(i<j); //i!=j会出错，想想是为什么
	a[i]=x;
	return i;
}

void quicksort(int a[],int m,int n){
	if(m<n){
		int q = partition(a,m,n);
		quicksort(a,m,q-1);
		quicksort(a,q+1,n);
	}
}

int main(){
	int a[34]={2,8,7,1,3,5,6,4,12,1,3,451,1,2,31,43,1,14,1,4,5,4,6,7,34,7,37,3,47,3,2,7,8,3};
	quicksort(a,0,33);
	for(int i=0;i<34;i++)
		printf("%d ",a[i]);
	printf("\n");
	return 0;
}










