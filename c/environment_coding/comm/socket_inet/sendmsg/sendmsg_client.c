#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
/*
 *2015年 10月 28日 星期三 11:33:23 CST
 *<nicksimon109445@gmail.com>
 *sendmsg client
 */
int main(int argc,char *argv[]){
	struct sockaddr_in serv;
	int s;
	char buf1[]="abc",buf2[]="bcd",buf3[]="cde\n";
	struct msghdr msg;
	struct iovec vector[3];
	struct iovec iov;
	iov.iov_base=buf1;
	iov.iov_len=strlen(buf1)+1;
	size_t len;
	vector[0].iov_base=buf1;
	vector[0].iov_len=strlen(buf1)+1;
	vector[1].iov_base=buf2;
	vector[1].iov_len=strlen(buf2)+1;
	vector[2].iov_base=buf3;
	vector[2].iov_len=strlen(buf3)+1;
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(9888);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	bzero(&msg,sizeof(struct msghdr));
	msg.msg_name=&serv;
	msg.msg_namelen=sizeof(struct sockaddr_in);
	msg.msg_iov=&iov;
	msg.msg_iovlen=1;
	if((s=socket(AF_INET,SOCK_DGRAM,0))<0){
		perror("socket error\n");
		return -1;
	}
	len=sendmsg(s,&msg,0);
	if(len<0){
		perror("sendmsg error\n");	
		close(s);
		return -1;
	}
	return 0;
}
