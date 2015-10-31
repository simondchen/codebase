#include <stdio.h>
class Solution{
	public:
		double findMedianSortedArrays(int A[],int m,int B[],int n){
			if((m+n)%2){	
				return (double)findKth(A,0,m-1,B,0,n-1,(m+n+1)/2);
			}else{
				double first = (double)findKth(A,0,m-1,B,0,n-1,(m+n)/2);
				double second = (double)findKth(A,0,m-1,B,0,n-1,(m+n)/2+1);
				return (double)(first+second)/2;
			}
		}
		
		int findKth(int A[],int aBeg,int aEnd,int B[],int bBeg,int bEnd,int k){
			if(aBeg>aEnd)	
			{
				return B[bBeg+k-1];
			}
			if(bBeg>bEnd)
			{
				return A[aBeg+k-1];
			}
			int aMid = aBeg+(aEnd-aBeg)/2;
			int bMid = bBeg+(bEnd-bBeg)/2;
			int halflen = aMid-aBeg+bMid-bBeg+2;
			if(A[aMid]<B[bMid]){
				if(halflen>k){
					return findKth(A,aBeg,aEnd,B,bBeg,bMid-1,k);
				}else{
					return findKth(A,aMid+1,aEnd,B,bBeg,bEnd,k-(aMid-aBeg+1));
				}
			}else{
				if(halflen>k){
					return findKth(A,aBeg,aMid-1,B,bBeg,bEnd,k);
				}else{
					return findKth(A,aBeg,aEnd,B,bMid+1,bEnd,k-(bMid-bBeg+1));
				}
			}
		}
};

int main(){
	int A[3]={1,3,5};
	int B[2]={2};
	Solution *s = new Solution();
	printf("%f\n",s->findMedianSortedArrays(A,3,B,1));
	return 0;
}
