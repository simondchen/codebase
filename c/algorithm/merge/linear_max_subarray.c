#include <stdio.h>

struct ret{
	int left;
	int right;
	int sum;
};

struct ret max_subarray(int *A){
	int n=A[0];
	int sum=0;
	int max=-500;
	int i;
	struct ret rt;
	int max_left,max_right;
	int left=1;
	//先扫描一遍，特殊对待全为负数的情况	
	for(i=1;i<=n;i++){
		if(A[i]>max){	
			max=A[i];
			max_left=max_right=i;
		}
	}
	if(max<=0){
		rt.left=rt.right=max_left;
		rt.sum=max;
		return rt;
	}
	max=0;
	for(i=1;i<=n;i++){
		if(A[i]>0){
			sum+=A[i];
		}else{
			if(sum>max){
				max = sum;
				max_left=left;
				max_right=i-1;
			}
			if(sum+A[i]<=0){
				sum=0;
				left=i+1;
			}else{
				sum+=A[i];
			}
		}
	}
	//结束是仍要判断一下
	if(sum>max){
		max=sum;
		max_left=left;
		max_right=i-1;
	}
	rt.left=max_left;
	rt.right=max_right;
	rt.sum=max;
	return rt;
}

int main(void){
	//int A[17]={16,13,-3,-25,20,-3,-16,-23,18,20,-7,12,-5,-22,15,-4,7};
	//int A[7]={6,-2,-1,3,5,-3,7};
	int A[4]={3,1,-1,3};
	struct ret rt=max_subarray(A);
	printf("(%d,%d,%d)\n",rt.left,rt.right,rt.sum);
	return 0;
}
