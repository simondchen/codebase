#include <stdio.h>
struct ret{
	int left;
	int right;
	int sum;
};

struct ret cross_max_subarray(int *A,int l,int mid,int r){
	int i;
	int left,left_max=-500,right,right_max=-500;
	int sum=0;
	struct ret rt;
	for(i=mid;i>=l;i--){
		sum+=A[i];	
		if(sum>left_max){
			left_max = sum;
			left = i;
		}
	}
	sum = 0;
	for(i=mid+1;i<=r;i++){
		sum+=A[i];	
		if(sum>right_max){
			right_max = sum;
			right = i;
		}
	}
	rt.left = left;
	rt.right = right;
	rt.sum = left_max + right_max;
	return rt;
}

struct ret merge_max_subarray(int *A,int l,int r){
	struct ret lt,rt,cross;
	if(l==r){	
		rt.left=rt.right=l;	
		rt.sum=A[l];
		return rt;
	}
	int mid = (l+r)/2;
	lt = merge_max_subarray(A,l,mid);
	rt = merge_max_subarray(A,mid+1,r);
	cross = cross_max_subarray(A,l,mid,r);
	if(lt.sum >= rt.sum && lt.sum >= cross.sum){
		return lt;
	}
	if(rt.sum >= lt.sum && rt.sum >= cross.sum){
		return rt;
	}
	return cross;	
}

int main(void){
	int A[17]={16,13,-3,-25,20,-3,-16,-23,18,20,-7,12,-5,-22,15,-4,7};
	struct ret r=merge_max_subarray(A,1,16);
	printf("(%d,%d,%d)\n",r.left,r.right,r.sum);
	return 0;
}
