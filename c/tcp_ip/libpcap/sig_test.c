#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float qp;
float qs;

float mean(int *h,int size){
	int sum=0;
	int i;
	for(i=0;i<size;i++)
	{
		sum+=h[i];
	}
	return sum/(float)size;
}
float sig(int *h,int size,float mean){
	float sum=0;
	int i;
	for(i=0;i<size;i++)
	{
		sum+=pow((h[i]-mean),2);
	}
	sum=sum/(float)size;
	return sqrt(sum);
}
int main(){
	//int h[6]={73,72,71,69,68,67};
	float m;
	float sv;
	int size=0,i;
	FILE *file;
	int h[100];
	int q[100];
	int p[100];
	float alpha=0.5;
	int qn=0,pn=0;
	//read from stdin
	/*printf("please input the num of numbers:\n");
	scanf("%d",&size);
	if((h=malloc(size*sizeof(int)))==NULL)
	{
		printf("malloc error\n");
		return -1;
	}
	printf("please input %d numbers:\n",size);
	for(i=0;i<size;i++){
		scanf("%d",&h[i]);
	}*/
	//read from a file
	//file=fopen("./data","r");
	file=fopen("./data2","r");
	if(file==NULL)
	{
		printf("read file error\n");
		return -1;
	}
	while(1){
		int number;
		fscanf(file,"%d",&number);
		if(feof(file))
			break;
		h[size++]=number;
	}
	m=mean(h,size);
	sv=sig(h,size,m);
	qp=m+alpha*sv;
	qs=m-alpha*sv;
	for(i=0;i<size;i++){
		if(h[i]>qp)
			q[qn++]=h[i];
		else if(h[i]<qs)
			p[pn++]=h[i];
		else
			printf("%d ",h[i]);
	}
	printf("\n");
	printf("%.2f\n",m);
	printf("%.2f\n",sv);
	printf("%.2f\n",qp);
	printf("%.2f\n",qs);
	for(i=0;i<qn;i++)
		printf("%d ",q[i]);
	printf("\n");
	for(i=0;i<pn;i++)
		printf("%d ",p[i]);
	printf("\n");
	return 0;
}
