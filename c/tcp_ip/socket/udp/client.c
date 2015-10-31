#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#define SERV_PORT 8000

int main(void){
	struct sockaddr_in serv;
	int sd;
	char buff[1024] = "hello,server";
	int len = strlen("hello,server")+1;
	size_t size;
	//1.设置服务器地址
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(SERV_PORT);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	
	//2.创建客户端套接字
	if((sd=socket(AF_INET,SOCK_DGRAM,0))<0){
		printf("create socket error\n");
		return -1;
	}

	//3.向服务器端发送消息，这个过程会自动分配port
	size = sendto(sd,buff,len,0,(struct sockaddr *)&serv,sizeof(struct sockaddr));
	if(size<0){
		printf("sendto error\n");
		return -1;
	}
	close(sd);	
	return 0;
}
