#include <stdio.h>
#include <math.h>

int findmax(int p[],int len){
	int i;
	int sum=0;
	int submax=0;
	for(i=0;i<len;i++){
		if(p[i]>0){	
			sum += p[i];
		}else{
			//遇到负数，sum值会减小，所以将sum值存下来
			if(sum>submax)
				submax=sum;
			if((sum+p[i])<0){
				sum=0;
			}else{
				sum += p[i];
			}
		}
	}
	if(sum>submax){
		submax=sum;
	}
	return submax;
}

int main(void){
	//int a[10] = {1,3,-2,2,9,-8,5,7,-84,10};
	int a[3]={1,-2,3};
	int sum;
	sum = findmax(a,3);
	printf("%d\n",sum);
	return 0;
}
