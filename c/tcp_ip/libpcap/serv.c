#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/time.h>
#define SERV_PORT 9877
int main(){
	struct sockaddr_in serv,cliaddr;
	int sd;
	int len;
	int n=0,count=50,num=0;
	char *buf="reply";
	char recv[1024];
	if((sd=socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		printf("sock error\n");
		return -1;
	}
	bzero(&serv,sizeof(serv));
	serv.sin_family=AF_INET;	
	serv.sin_port=htons(SERV_PORT);
	serv.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(sd,(struct sockaddr *)&serv,sizeof(serv))<0){
		printf("bind error\n");
		return -1;
	}
	int recvBufSize;
	//int err=getsockopt(sd,SOL_SOCKET,SO_RCVBUF,(char *)&recvBufSize,sizeof(recvBufSize));
	//printf("size:%d\n",recvBufSize);
	while(count-->0){	
		n=recvfrom(sd,recv,1024,0,(struct sockaddr *)&cliaddr,&len);
		if(n<0)
		{
			printf("recv error\n");
			return -1;
		}
		printf("%d\n",num++);
		sendto(sd,buf,strlen(buf),0,(struct sockaddr *)&cliaddr,len);
	}
	return 0;
}
