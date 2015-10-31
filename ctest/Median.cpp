#include <iostream>
using namespace std;
class Solution{
	public:
		double findMedianSortedArrays(int A[],int m,int B[],int n){
			//int len=m+n; 	
			return 0;
		}
};

double singleMedia(int B[],int n){
	if(n%2) //奇数
		return B[n/2];		
	else
		return (double)(B[(n/2-1)]+B[n/2])/2;
}

double findMedian(int A[],int &m,int B[],int &n,int &index,int &left,int &start){
	int media = singleMedia(B+start,n);
	if(media==)
	if((index+left))
	return 0;	
}

int retArr(int B[],int n){
	return B[n];
}

int main(){
	int b[] = {1,2,3,4,5};
	cout<<retArr(b+1,0)<<endl;
	return 0;
}
