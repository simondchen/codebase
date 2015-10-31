#include <stdio.h>

void max_heap_keep(int a[],int i,int n){
	//这里穿进来的是一个指针，所以sizeof(a)会出错
	int left = 2*i; // what a shit,left=2i,这样完全不是表达式呀！！！
	int right = 2*i+1;
	int largest = i;
	if(left<=n && a[left-1]>a[i-1]){	
		largest = left;
	}
	if(right<=n && a[right-1]>a[largest-1]){
		largest = right;
	}
	if(largest!=i){
		int tmp = a[largest-1];
		a[largest-1] = a[i-1];
		a[i-1] = tmp;
		max_heap_keep(a,largest,n);
	}
}

void max_heap(int a[],int len){
	int j;
	for(j=len/2;j>=1;j--){
		max_heap_keep(a,j,len);	
	}
}

void heapsort(int a[],int len){
	//构建最大堆
	max_heap(a,len);	
	int j;
	for(j=len;j>=2;j--){
		int tmp;
		tmp = a[j-1];
		a[j-1]=a[0];
		a[0] = tmp;
		max_heap_keep(a,1,j-1);
	}
}

int main(){
	int a[] = {1,2,3,4,6,1,3,2,6,32};
	int len = sizeof(a)/sizeof(int);
	heapsort(a,len);
	int i;
	for(i=0;i<len;i++){
		printf("%d\n",a[i]);
	}
	return 0;
}
