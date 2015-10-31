#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <fcntl.h>
/*
 *2015年 10月 25日 星期日 07:35:40 CST
 *<nicksimon109445@gmail.com>
 *a tcp socket server using select
 */
int main(int argc,char *argv[]){
	struct sockaddr_in serv;
	int socket_master,cld,size,fd;
	int clds[10];
	char buf[1024];
	int i;
	int maxfd;
	struct timeval tv;
	fd_set rdfs;
	for(i=0;i<10;i++){
		clds[i]=0;
	}
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(9888);
	//serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	if((socket_master=socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket error\n");
		return -1;
	}
	if(bind(socket_master,(struct sockaddr *)&serv,sizeof(struct sockaddr_in))<0){
		perror("bind error\n");
		close(socket_master);
		return -1;
	}
	if(listen(socket_master,3)<0){
		perror("listen error\n");
		close(socket_master);
		return -1;
	}
	while(1){
		FD_ZERO(&rdfs);
		FD_SET(socket_master,&rdfs);
		maxfd=socket_master;
		for(i=0;i<10;i++){
			fd=clds[i];
			if(fd!=0){
				FD_SET(fd,&rdfs);
				if(fd>maxfd)
					maxfd=fd;
			}
		}
		tv.tv_sec=5;
		tv.tv_usec=0;
		//select会重新设置time,而其值是不确定的!!!所以要手动的重新设置
		int ret=select(maxfd+1,&rdfs,NULL,NULL,&tv);
		if(ret<0){
			perror("select error\n");
			goto fail;
		}
		else if(ret==0){
			printf("no data\n");
		}
		else{
			if(FD_ISSET(socket_master,&rdfs)){
				cld=accept(socket_master,NULL,NULL);
				if(cld<0){
					perror("accept error\n");
					close(socket_master);
					return -1;
				}
				printf("connection comes\n");
				for(i=0;i<10;i++){
					if(clds[i]==0){
						clds[i]=cld;
						break;
					}
				}
				//将cld设置成非阻塞
				int flags=fcntl(cld,F_GETFL,0);
				if(fcntl(cld,F_SETFL,flags | O_NONBLOCK)<0){
					perror("fcntl error\n");	
					goto fail;
				}
			}
			for(i=0;i<10;i++){
				fd=clds[i];
				if(fd!=0){
					//printf("new data ready\n");
					if(FD_ISSET(fd,&rdfs)){
						while((size=read(fd,buf,1024))>0){
							printf("size:%d\n",size);
						//while(size=recv(fd,buf,1024,MSG_DONTWAIT)){
							write(1,buf,size);
						}
						//printf("out of recv\n");
						if(size<0){
							if(errno==EAGAIN || errno==EWOULDBLOCK)
							{
								//printf("nonblock\n");
								continue;
							}
							else{
								perror("read error\n");
								goto fail;
							}
						}
						if(size==0){
							printf("size is zero\n");
							//connection closed
							clds[i]=0;
						}
					}
				}
			}
		}
	}
	return 0;
	fail:
		close(socket_master);
		for(i=0;i<10;i++){
			if(clds[i]!=0)
				close(clds[i]);
		}
}
