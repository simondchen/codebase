#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>
#include <sys/un.h>
#include <errno.h>
#include <msg.h>
/*
 *2015年 10月 24日 星期六 15:58:53 CST
 *<nicksimon109445@gmail.com>
 *a daemon server exploting multiplex to handle multi connections
 */
#define MAX_CONN_NUM 3
#define MAX_CLIENT_NUM 20

int main(int argc,char *argv[]){
	const char *servname = "foo.sock";
	struct sockaddr_un serv;	
	int size,ret,master_socket,cld,maxfd=0,i;
	//初始化为零
	int clients[MAX_CLIENT_NUM];
	char buf[1024];
	int opt=1;
	fd_set rdfs;
	socklen_t len;		
	/**********/
	//设置标准输出流为无缓冲
	setvbuf(stdout,NULL,_IONBF,0);

	for(i=0;i<MAX_CLIENT_NUM;i++){
		clients[i]=0;
	}

	if((master_socket=socket(AF_UNIX,SOCK_STREAM,0))<0){
		perror("socket error:\n");
		goto fail;
	}
	/*********/
	 if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
     {
        perror("setsockopt");
		goto fail;
     }

	/**********/
	memset(&serv,0,sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	strcpy(serv.sun_path,servname);
	len=offsetof(struct sockaddr_un,sun_path)+strlen(servname)+1;
	if((ret=bind(master_socket,(struct sockaddr*)&serv,len))<0){
		perror("bind error:\n");
		goto fail;
	}
	/*********/

	/********/
	if((ret=listen(master_socket,MAX_CONN_NUM))<0){
		perror("listen error:\n");
		goto fail;
	}
	/********/
	struct timeval tv;
	//主循环
	while(1){
		//每次要重新设置fdset!!!
		FD_ZERO(&rdfs);
		FD_SET(master_socket,&rdfs);
		maxfd=master_socket;
		for(i=0;i<MAX_CLIENT_NUM;i++){
			if(clients[i]==0)
				continue;
			FD_SET(clients[i],&rdfs);
			if(clients[i]>maxfd)
				maxfd=clients[i];
		}
		tv.tv_sec=5;
		tv.tv_usec=0;
		ret=select(maxfd+1,&rdfs,NULL,NULL,&tv);
		if(ret<0){
			perror("select error\n");
			goto fail;
		}
		else if(ret==0){
			printf("no data availabe\n");
		}
		else{
			if(FD_ISSET(master_socket,&rdfs)){
				//master_socket有数据来了,说明有新的链接到达
				printf("a new connection comes\n");
				if((cld=accept(master_socket,NULL,NULL))<0){
					perror("accept error\n");
					goto fail;
				}
				for(i=0;i<MAX_CLIENT_NUM;i++){
					if(clients[i]==0){
						clients[i]=cld;
						if(cld>maxfd)
							maxfd=cld;
						FD_SET(cld,&rdfs);
						break;
					}
				}
				//说明超过最大连接个数,不接受,关闭cld
				if(i==MAX_CLIENT_NUM){
					printf("exceeding the max conn num,do not accept it\n");
					close(cld);
				}
			}
			//处理客户端传来的数据
			for(i=0;i<MAX_CLIENT_NUM;i++){
				if(clients[i]==0)
					continue;
				//该cld有数据到达
				if(FD_ISSET(clients[i],&rdfs)){
					//这里的设置是关健:1.处理断开连接的情况2.设置成nonblock
					while((size=recv(clients[i],buf,1024,MSG_DONTWAIT))>0){
						struct mymsghdr *msg=(struct mymsghdr *)buf;
						//注意这里printf是行缓冲的,所以不会打印出来,不要将标准I/O流和read,write弄混了
						printf("message from %5d:",msg->pid);
						write(1,msg->data,msg->len);
					}
					if(size<0){
						if(errno==EAGAIN || errno==EWOULDBLOCK){
							continue;
						}
						else{
							perror("read error\n");
							goto fail;
						}
					}
					//处理断开连接的情况
					if(size==0){
						printf("a connection closed\n");
						clients[i]=0;
					}
				}
			}
		}
	}


	fail:
		for(i=0;i<MAX_CLIENT_NUM;i++){
			if(clients[i])
				close(clients[i]);
		}
		close(master_socket);
		return -1;
}
