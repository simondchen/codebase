#include <stdio.h>
int FindKthNum(int a[],int aBeg,int aEnd,int b[],int bBeg,int bEnd,int k){
	if(aBeg>aEnd){
		return b[bBeg+k-1];
	}
	if(bBeg>bEnd){
		return a[aBeg+k-1];
	}
	int aMid=aBeg+(aEnd-aBeg)/2;
	int bMid=bBeg+(bEnd-bBeg)/2; 
	int halflen=aMid-aBeg+bMid-bBeg+2;
	if(aMid>bMid){
		if(halflen>k)
			return FindKthNum(a,aBeg,aMid-1,b,bBeg,bEnd,k);
		else
			return FindKthNum(a,aBeg,aEnd,b,bMid+1,bEnd,k-bMid+bBeg-1);
	}else{
		if(halflen>k)
			return FindKthNum(a,aBeg,aEnd,b,bBeg,bMid-1,k);
		else
			return FindKthNum(a,aMid+1,aEnd,b,bBeg,bEnd,k-aMid+aBeg-1);
	}
}
