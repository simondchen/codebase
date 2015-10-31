#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>

#define SERV_PORT 9877

int main(int argc,char **argv){
	int sd;
	int n=0,count=50,num=0;
	struct sockaddr_in serv,recvaddr;
	char *buf="request";
	char recv[1024];
	if(argc!=2)
	{
		printf("usage:updcli serv_addr\n");
		return -1;
	}
	bzero(&serv,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(SERV_PORT);
	serv.sin_addr.s_addr=inet_addr(argv[1]);
	if((sd=socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		printf("socket error\n");
		return -1;
	}
	while(count-->0){	
		//usleep(50000);
		usleep(50000);
		if(sendto(sd,buf,strlen(buf),0,(struct sockaddr *)&serv,sizeof(serv))<0)
		{
			printf("send error\n");
			return -1;
		}
		n=recvfrom(sd,recv,1024,0,NULL,NULL);
		if(n<0)
		{
			printf("recv error\n");
			return -1;
		}
		printf("%d\n",num++);
	}
	return 0;
}
