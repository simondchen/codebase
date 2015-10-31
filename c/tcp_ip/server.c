#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
int main(){
	int sd,cd;
	//socklen_t len;
	char *buf="hello client\n";
	struct sockaddr_in serv;
	if((sd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("socket error\n");
		return -1;
	}
	bzero(&serv,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_addr.s_addr=htonl(INADDR_ANY);
	serv.sin_port=htons(8888);
	if(bind(sd,(struct sockaddr *)&serv,sizeof(serv))<0)
	{
		printf("bind error\n");
		return -1;
	}
	if(listen(sd,3)<0)
	{
		printf("listen error\n");
		return -1;
	}
	for(;;)
	{
		if((cd=accept(sd,NULL,NULL))<0)
		{
			printf("accept error\n");
			return -1;
		}
		printf("a client comes\n");
		write(cd,buf,strlen(buf));
		close(cd);
	}
}
