#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
/*
 *2015年 10月 25日 星期日 07:50:28 CST
 *<nicksimon109445@gmail.com>
 *a select socket test client
 */
int main(int argc,char *argv[]){
	struct sockaddr_in serv;
	int cld;
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(9888);
	//serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	inet_aton("127.0.0.1",(struct in_addr *)&(serv.sin_addr));
	if((cld=socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket error\n");
		return -1;
	}
	if(connect(cld,(struct sockaddr *)&serv,sizeof(struct sockaddr_in))<0){
		perror("connect error\n");
		close(cld);
		return -1;
	}
	int flags = fcntl(cld,F_GETFL,0);
	if(fcntl(cld,F_SETFL,flags|O_NONBLOCK)<0){
		perror("fcntl error\n");
		close(cld);
		return -1;
	}
	char buf[1024];
	sprintf(buf,"hello server:%5d\n",getpid());
	while(1){
		write(cld,buf,strlen(buf)+1);
		sleep(2);
	}
	close(cld);
	return 0;
}
