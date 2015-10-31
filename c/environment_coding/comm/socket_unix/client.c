#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <msg.h>
#include <stdlib.h>
/*
 *2015年 10月 24日 星期六 10:38:30 CST
 *<nicksimon109445@gmail.com>
 *UNIX domain socket client
 */
int main(int argc,char *argv[]){
	const char *servname="foo.sock";
	struct sockaddr_un serv;
	int sd,ret;
	socklen_t len;
	//1.创建socket套接字
	if((sd=socket(AF_UNIX,SOCK_STREAM,0))<0){
		perror("socket error\n");
		goto fail;
	}
	//2.设置服务器地址
	memset(&serv,0,sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	sprintf(serv.sun_path,"sk%5d",getpid());
	printf("%5d\n",getpid());
	printf("%s\n",serv.sun_path);
	unlink(serv.sun_path);
	//3.将socket bind 到某一地址,这里不要绑定到和服务器相同的地址
	len=offsetof(struct sockaddr_un,sun_path)+strlen(servname)+1;
	if((ret=bind(sd,(struct sockaddr *)&serv,len))<0){
		perror("bind error\n");
		goto fail;
	}
	//4.connect到服务器
	//先设置服务器地址
	memset(&serv,0,sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	//dest must be large enough
	strcpy(serv.sun_path,servname);
	len=offsetof(struct sockaddr_un,sun_path)+strlen(servname)+1;
	if((ret=connect(sd,(const struct sockaddr *)&serv,len))<0){
		perror("connect error\n");
		goto fail;
	}
	struct mymsghdr *msg=NULL;
	char buf[1024];
	while(1){
		//这里将输入改成从标准输入获取
		fgets(buf,1024,stdin);
		int size=strlen(buf)+1;
		msg=(struct mymsghdr *)malloc(sizeof(struct mymsghdr)+size-1);
		msg->pid=getpid();
		msg->len=size;
		memcpy(msg->data,buf,size);
		write(sd,msg,sizeof(struct mymsghdr)+size-1);
	}
	close(sd);
	return 0;
	fail:
		close(sd);
		return -1;
}
