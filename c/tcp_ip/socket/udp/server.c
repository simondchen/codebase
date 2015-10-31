#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <strings.h>

#define SERV_PORT 8000

int main(void){
	struct sockaddr_in serv;
	struct sockaddr_in client;
	int clen;
	int sd;
	char buff[1024];
	size_t size;
	//1.设置服务器地址
	//清零
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	//注意这里要转换成网络字节序
	serv.sin_port = htons(SERV_PORT);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	//2.创建套接字	
	if((sd = socket(AF_INET,SOCK_DGRAM,0))<0){
		printf("create socket error\n");
		return -1;
	}

	//3.绑定套接字到指定地址
	if(bind(sd,(struct sockaddr *)&serv,sizeof(struct sockaddr))<0){
		printf("bind error\n");
		return -1;
	}
	
	//4.等待客户端发送数据
	size = recvfrom(sd,buff,1024,0,(struct sockaddr *)&client,(socklen_t *)&clen);	
	if(size < 0){	
		printf("recvfrom error\n");
		return -1;
	}
	printf("%s\n",buff);
	
	//5.关闭socket
	return 0;
}
