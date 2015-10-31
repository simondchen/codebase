#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#define PORT 9889
int main(){
	int sd,cd;
	struct sockaddr_in serv;
	int buf[512],size,num=0,len,*h;
	int llen;
	if((sd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("socket error\n");
		return -1;
	}
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(PORT);
	serv.sin_addr.s_addr=htonl(INADDR_ANY);
	//serv.sin_addr=inet_addr(INADDR_ANY);
	if(bind(sd,(struct sockaddr *)&serv,sizeof(struct sockaddr))<0){
		printf("bind error\n");
		return -1;
	}
	if(listen(sd,3)<0)
	{
		printf("listen error\n");
		return -1;
	}
	cd=accept(sd,NULL,NULL);
	if(cd<0){
		printf("accept error\n");
		return -1;
	}
	//size of char!!!
	//if two write is one by one,then data will be stored in the buffer,and read will obtain data from two-time write
	size=read(cd,&llen,sizeof(int));
	len=llen;
	h=malloc(len*sizeof(int)+1);
	if(h==NULL)
	{
		printf("malloc error\n");
		return -1;
	}
	h[0]=len;
	size=read(cd,h+1,(len)*sizeof(int));
	printf("%d\n",size);	
	int i;
	for(i=0;i<len+1;i++)
	{
		printf("%d\n",h[i]);
	}
	close(sd);
	return 0;
}
