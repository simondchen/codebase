#include <stdio.h>
int findnum(int *a,int beg,int end,int num){
	if(beg==end)
		return beg;
	int mid = beg+(end-beg)/2;	
	if(a[mid]<num)
	{	
		beg = mid+1;
		findnum(a,beg,end,num);
	}else if(a[mid]>num){
		end=mid-1;	
		findnum(a,beg,end,num);
	}else{
		return mid;
	}
}
int main(){
	int num;
	int i;
	int a[15];
	for(i=0;i<15;i++)
	{
		a[i]=2*i+1;
	}
	printf("please enter the num you want to find:");
	scanf("%d",&num);
	printf("%d\n",findnum(a,0,14,num));
	return 0;
}
