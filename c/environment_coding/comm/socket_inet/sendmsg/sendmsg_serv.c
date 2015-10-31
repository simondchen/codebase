#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
/*
 *2015年 10月 28日 星期三 11:15:29 CST
 *<nicksimon109445@gmail.com>
 *practice how to use sendmsg
 */
int main(int argc,char *argv[]){
	struct sockaddr_in serv;
	struct sockaddr_in client;
	int s;
	struct msghdr msg;
	size_t len;
	struct iovec iov;
	char buf[20];
	memset(buf,0,20);
	iov.iov_base=buf;
	iov.iov_len=20;
	bzero(&msg,sizeof(struct msghdr));	
	msg.msg_name=&client;
	msg.msg_namelen=sizeof(struct sockaddr_in);
	msg.msg_iov=&iov;
	msg.msg_iovlen=1;
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(9888);
	serv.sin_addr.s_addr=htonl(INADDR_ANY);
	if((s=socket(AF_INET,SOCK_DGRAM,0))<0){
		perror("socket error\n");
		return -1;
	}
	if(bind(s,(struct sockaddr *)&serv,sizeof(serv))<0){
		perror("bind error\n");
		close(s);	
		return -1;
	}
	//不需要listen
	//if(listen(s,3)<0)
	//char buf[1024];
	//len=read(s,buf,1024);
	len=recvmsg(s,&msg,0);
	if(len<0){
		perror("recvmsg error\n");
		close(s);
		return -1;
	}
	if(len==0){
		printf("no msg\n");
	}
	if(len>0){
		printf("msg comes\n");
		//writev(1,msg.msg_iov,msg.msg_iovlen);
		printf("%s\n",msg.msg_iov->iov_base);
	}
	return 0;
}
