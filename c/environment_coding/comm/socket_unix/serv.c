#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#define MAX_CONN_NUM 3
/*
 *2015年 10月 24日 星期六 09:58:34 CST
 *<nicksimon109445@gmail.com>
 *a UNIX domain socket server
 */
int main(int argc,char *argv[]){
	char *servname="foo.sock";
	struct sockaddr_un un;
	struct sockaddr_un cli;
	//typedef __socklen_t socklen_t, typedef unsigned int __socklen_t
	socklen_t len,cli_len;
	int sd,cld,ret;
	//1.创建socket
	if((sd=socket(AF_UNIX,SOCK_STREAM,0))<0){
		perror("socket create error\n");
		goto fail;
	}
	//2.设置地址
	memset(&un,0,sizeof(struct sockaddr_un));
	un.sun_family = AF_UNIX;
	//strcpy is not safe,use strncpy instread,use strcpy,you must make sure that is dest buffer is large enough!!!
	//strcpy(un.sun_path,servname);
	strncpy(un.sun_path,servname,strlen(servname)+1);
	//3.bind到设置的地址
	len=offsetof(struct sockaddr_un,sun_path)+strlen(servname)+1;
	if((ret=bind(sd,(struct sockaddr *)&un,len))<0){
		perror("bind error\n");
		goto fail;
	}
	//4.listen等待连接到来
	if((ret=listen(sd,MAX_CONN_NUM))<0){
		perror("listen error\n");
		goto fail;	
	}
	//5.accpet客户端的连接
	//要记得初始化cli和cli_len,因为从客户端接收过来的未必填充全部sockaddr_un,一般都填充不满,所以不设置其内存内容将无法确定!!!
	memset(&cli,0,sizeof(struct sockaddr_un));
	cli_len=1;
	//这里写错啦!!!accept会返回一个新的file descriptor用于与客户端通信
	if((cld=accept(sd,(struct sockaddr *)&cli,(socklen_t *)&cli_len))<0){
		perror("accept error\n");
		goto fail;
	}
	printf("client connection comes\n");
	//6.接收客户端传过来的数据
	int size;
	char buf[1024];
	memset(buf,0,1024);
	if((size=read(cld,buf,1024,0))>0){
		//write(1,buf,size);
		printf("%s",buf);
	}
	if(size<0){
		perror("read error\n");
		goto fail;
	}
	close(sd);
	return 0;

	//出错处理,释放socket
	fail:
		close(sd);	
		return -1;
}
