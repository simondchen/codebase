#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#define SERV_PORT 9877
int main(){
	struct sockaddr_in serv,cliaddr;
	int sd;
	int len;
	int n=0;
	int num=0;
	char *buf="b";
	char recv[2];
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
	while(1){
		if((n=recvfrom(sd,recv,2,0,(struct sockaddr *)&cliaddr,&len))<0)
		{
			printf("recv error\n");
			return -1;
		}
		sendto(sd,buf,strlen(buf),0,(struct sockaddr *)&cliaddr,len);
		num++;
		printf("%d\n",num);
	}
}
