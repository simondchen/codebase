#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#define PORT 9888
#define PATH "./sample"

int m=4;
float qp,qs;

int quant(int h){
	if(h>qp)
	{
		return 1;
	}else if(h<qs) /*how careless you are,write qs as qp*/
	{
		return 0;
	}else
	{
		return -1;
	}
}

int key_gen(int *h,int *sset,int **ka){
	int i,len;
	len=sset[0];
	//int *key,not capable,allocated in stack,useless out of the func 
	int *key=malloc(len*sizeof(int));
	if(key==NULL){
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

int getlines(FILE *fp){
	int ch;	
	int lines=0;
	while((ch=fgetc(fp))!=EOF)
	{
		if(ch=='\n')
			lines++;
	}
	return lines;
}

int seq(int *h,int size,int *lset,int **sset){
	int lsize=lset[0];
	int i;
	int num=1;
	int *set=malloc(lsize*sizeof(int));
	if(set==NULL){
		printf("malloc set error\n");
		return -1;
	}
	//for each index l,check at least m-1 channel estimates centered around that index
	for(i=1;i<=lsize;i++){
		int l=lset[i];
		int j;
		//assume m is even
		int q=quant(h[l]);
		if(q==-1)
		{
			continue;			
		}
		for(j=(l-(m-2)/2);j<=(l+m/2);j++)
		{
			if(q!=quant(h[j]))
			{
				break;
			}
		}
		//j==(l+m/2+1) indicates break is not executed and the loop is fully executed!
		if(j==(l+m/2+1))
		{
			set[num++]=l;
		}
	}
	set[0]=num-1;
	*sset=set;
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

int main(){
	//socket recv logic
	FILE *file;	
	int i,lines,split=':',line=0,llen,size,*lset,*sset,*ka;
	char buf[512];
	char *pos;
	//socket members
	struct sockaddr_in serv;
	int sd,cd;
	int error=0;
	float mean,sv,alpha=0.5;
	//open bob's seq file
	if((file=fopen(PATH,"r"))==NULL)
	{
		printf("open file error\n");
		return -1;
	}
	//copy content in file to memory
	lines=getlines(file);
	int *h=malloc(lines*sizeof(int));
	if(h==NULL)
	{
		printf("malloc error\n");
		return -1;
	}
	fseek(file,0,SEEK_SET);
	while(fgets(buf,sizeof(buf),file)){
		pos=strrchr(buf,split);	
		if(pos==NULL)
		{
			printf("bad format\n");
			return -1;
		}
		buf[strlen(buf)-3]='\0';
		pos+=2;
		h[line++]=atoi(pos);
		printf("%d\n",h[line-1]);
	}
	//compute mean,qp and qs
	mean=c_mean(h,lines);
	sv=c_sig(h,lines,mean);
	printf("%.2f\n",mean);
	printf("%.2f\n",sv);
	qp=mean+alpha*sv;
	qs=mean-alpha*sv;
	printf("%.2f\n",qp);
	printf("%.2f\n",qs);
	//socket init
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(PORT);
	serv.sin_addr.s_addr=htonl(INADDR_ANY);
	if((sd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("socket error\n");
		return -1;
	}
	if(bind(sd,(const struct sockaddr*)&serv,sizeof(struct sockaddr))<0){
		printf("bind error\n");
		return -1;	
	}
	if(listen(sd,3)<0)
	{
		printf("listen error\n");
		return -1;
	}
	//block here
	cd=accept(sd,NULL,NULL);
	if(cd<0)
	{
		printf("accept error\n");
		return -1;
	}
	//read lset from alice and store it to lset with lset[0] indicates the length of lset 
	int len;
	read(cd,&len,sizeof(int));
	lset=malloc((len+1)*sizeof(int));
	printf("recv lset length:%d\n",len);
	lset[0]=len;
	if(lset==NULL)
	{
		printf("malloc error\n");
		return -1;
	}
	read(cd,lset+1,len*sizeof(int));
	for(i=1;i<=len;i++){
		printf("%d ",lset[i]);
	}
	printf("\n");
	if(error=seq(h,lines,lset,&sset))
	{
		printf("seq error\n");
		return -1;
	}
	write(cd,sset,(sset[0]+1)*sizeof(int));
	close(cd);
	close(sd);
	//quantitate the set to key 
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
