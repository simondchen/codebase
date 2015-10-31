#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#define PORT 9888
#define PATH "./sample"
//you did not init m,how careless you are
int m=4;
float qp,qs;

int quant(int h){
	if(h>qp){
		return 1;
	}else if(h<qs){
		return 0;
	}else{
		//invalid number
		return -1;
	}
}

int key_gen(int *h,int *sset,int **ka){
	int len=sset[0];
	int i;
	//int *key,this is not good,you should malloc size,or the key will lose its function out of the key_gen func 
	int *key=malloc(len*sizeof(int));
	if(key==NULL)
	{
		printf("key_gen malloc error\n");
		return -1;
	}
	for(i=1;i<=len;i++)
	{
		key[i-1]=quant(h[sset[i]]);
	}
	*ka=key;
	return 0;
}

int seq(int *h,int size,int **lset){  
	//please store the size of lset somwhere 
	int num=1;
	//alloc a space of size/4
	int *mbuf=malloc((size/4)*sizeof(int));
	if(mbuf==NULL){
		printf("malloc lset space error\n");
		return -1;
	}
	int i=0;
	//pay attention to such boundary
	while((i+m)<=size){
		int j;
		int q=quant(h[i]);
		if(q==-1)
		{
			i++;
			continue;
		}
		for(j=1;j<m;j++){
			if(q!=quant(h[i+j])){
				i++;
				//this is a bug,continue just jump out the loop it belongs to! 
				//continue;
				break;
			}
		}
		//j equals m indicates that loop is excuted fully,and not jump out from break; 
		if(j==m)
		{
			mbuf[num++]=(2*i+m-1)/2;	
			i+=m;
		}
	}
	//store array size in the beginning of the array
	mbuf[0]=num-1;
	*lset=mbuf;	
	return 0;
}

float c_mean(int *h,int size){
	int sum=0;
	int i;
	for(i=0;i<size;i++)
	{
		sum+=h[i];
	}
	return sum/(float)size;
}

float c_sig(int *h,int size,float mean){
	float sum=0;
	int i;
	for(i=0;i<size;i++)
	{
		sum+=pow((h[i]-mean),2);		
	}
	sum=sum/(float)size;
	return sqrt(sum);
}

int getlines(FILE *file){
	int ch;
	//init lines please!direct ++ without init is dangerous
	int lines=0;
	while((ch=fgetc(file))!=EOF)
	{
		if(ch=='\n')
			lines++;	
	}
	return lines;
}

int main(int argc,char *argv[]){
	FILE *file;
	int lines,i;
	char *pos;
	int split=':';
	char buf[512];
	float alpha=0.5;
	float mean;
	float sv;
	int *lset,*sset,*ka;
	//socket part variable statement
	struct sockaddr_in serv;
	int sd;
	if(argc<2)
	{
		printf("usage:./a.out servaddr\n");
		return -1;
	}
	if((file=fopen(PATH,"r"))==NULL)	
	{
		printf("open file error\n");
		return -1;
	}
	lines=getlines(file);	
	int *h=malloc(lines*sizeof(int));
	//int *h=malloc(8000*sizeof(int));
	int line=0;
	if(h==NULL)
	{
		printf("malloc h error\n");
		return -1;
	}
	//reset the file point
	fseek(file,0L,SEEK_SET);
	//printf("current file offset:%d\n",(int)ftello(file));
	while(fgets(buf,sizeof(buf),file)){
		pos=strrchr(buf,split);
		if(pos==NULL)
		{
			printf("wrong format\n");
			return -1;
		}
		buf[strlen(buf)-3]='\0';
		pos+=2;
		h[line++]=atoi(pos);
		printf("%d\n",h[line-1]);
	}
	mean=c_mean(h,lines);
	sv=c_sig(h,lines,mean);	
	printf("%.2f\n",mean);
	printf("%.2f\n",sv);
	qp=mean+alpha*sv;
	qs=mean-alpha*sv;
	printf("%.2f\n",qp);
	printf("%.2f\n",qs);
	if(seq(h,lines,&lset)<0)
	{
		printf("seq error\n");
		return -1;
	}
	//test lset
	printf("lset size:%d\n",lset[0]);
	for(i=1;i<=lset[0];i++)
	{
		printf("%d ",lset[i]);	
	}
	//socket logic to send L,so you had better check forward logic;
	//sockaddr init
	bzero(&serv,sizeof(struct sockaddr_in));	
	serv.sin_family=AF_INET;
	serv.sin_port=htons(PORT);
	serv.sin_addr.s_addr=inet_addr(argv[1]);
	if((sd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("socket error\n");
		return -1;
	}
	if(connect(sd,(const struct sockaddr *)&serv,sizeof(struct sockaddr))<0)
	{
		printf("connect error\n");
		return -1;
	}
	//send lset,the algorithm select a random subset of lset,we set lset directly here 
	write(sd,lset,(lset[0]+1)*sizeof(int));
	int len;
	printf("wait for sset from bob\n");
	read(sd,&len,sizeof(int));
	printf("sset from bob comes\n");
	printf("sset len:%d\n",len);
	sset=malloc((len+1)*sizeof(int));
	sset[0]=len;
	if(sset==NULL){
		printf("malloc sset error\n");
		return -1;
	}
	read(sd,sset+1,len*sizeof(int));
	//sset test
	for(i=1;i<=len;i++)
	{
		printf("%d ",sset[i]);
	}
	printf("\n");
	close(sd);
	//generate key
	if(key_gen(h,sset,&ka)==-1)
	{
		printf("key_gen error\n");
		return -1;
	}
	for(i=0;i<sset[0];i++){
		printf("%d",ka[i]);
	}
	printf("\n");
	return 0;
}
