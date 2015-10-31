#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/un.h>
//这个程序其实是失败的,具体原因还不是太清楚,但是线程的使用不熟,而且select并不是和线程一起搭配使用的
/*
 *2015年 10月 24日 星期六 15:58:53 CST
 *<nicksimon109445@gmail.com>
 *a daemon server exploting multiplex to handle multi connections
 */
#define MAX_CONN_NUM FD_SETSIZE
int sockfd_array[FD_SETSIZE];
int conn_num=0;
int maxfd=0;

void *process_fdset(void *params){
	fd_set readfs;	
	struct timeval tv={5,0};
	int ret,len,fd;
	int buf[1024];
	while(1){
		FD_ZERO(&readfs);		
		int i;
		for(i=0;i<conn_num;i++){
			FD_SET(sockfd_array[i],&readfs);
		}
		ret=select(maxfd+1,&readfs,NULL,NULL,&tv);
		if(ret<0){
			perror("select error\n");
			pthread_exit(NULL);
		}
		else if(ret=0){
			printf("no data available with 5 sec");
		}
		else{
			int j;
			//for(i=0,j=0;i<ret,j<conn_num;j++){
			for(j=0;j<conn_num;j++){
				fd=sockfd_array[j];
				if(FD_ISSET(fd,&readfs)){
					//i++;
					//数据处理的代码最好抽离出来,封装一下
					printf("message comes:");
					while((len=read(fd,buf,1024))>0){
						//write(1,buf,len);
					}
					if(len<0){
						perror("read error\n");
						pthread_exit(NULL);
					}
					//printf("\n");
				}
			}
		}
	}
}

int main(int argc,char *argv[]){
	const char *servname = "foo.sock";
	struct sockaddr_un serv;	
	int ret,sd,cld;
	socklen_t len;		
	/**********/
	if((sd=socket(AF_UNIX,SOCK_STREAM,0))<0){
		perror("socket error:\n");
		goto fail;
	}
	/*********/

	/**********/
	memset(&serv,0,sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	strcpy(serv.sun_path,servname);
	len=offsetof(struct sockaddr_un,sun_path)+strlen(servname)+1;
	if((ret=bind(sd,(struct sockaddr*)&serv,len))<0){
		perror("bind error:\n");
		goto fail;
	}
	/*********/

	/********/
	if((ret=listen(sd,MAX_CONN_NUM))<0){
		perror("listen error:\n");
		goto fail;
	}
	/********/

	//关键,fork子进程专门处理fdset,而父进程处理连接,进程不好用,因为不共享文件描述符,所以要使用线程!!!
	/*pid_t pid=fork();
	if(pid<0){
		perror("fork error\n");
		goto fail;
	}
	//父进程
	if(pid>0){
		while(1){
			//处理accept
			if(cld=accept())
		}
	}*/
	pthread_t tid;
	//注意线程不共享栈(stack),所以main中的局部变量线程无法访问,而fork的子进程是会赋值栈的
	if(pthread_create(&tid,NULL,process_fdset,NULL)<0){
		perror("pthread_create error\n");
		goto fail;
	}
	while(1){
		if((cld=accept(sd,NULL,NULL))<0){
			perror("accept error\n");
			goto thread_fail;
		}
		printf("one connection comes with cld:%d\n",cld);
		if(cld>maxfd){
			maxfd=cld;
		}
		//这段多线程操作全局变量应该加入互斥机制
		sockfd_array[conn_num]=cld;
		conn_num++;
		printf("conn_num:%d\n",conn_num);	
	}
	fail:
		close(sd);
		return -1;
	thread_fail:
		close(sd);
		int i;	
		for(i=0;i<conn_num;i++){
			close(sockfd_array[i]);
		}
		pthread_exit(NULL);
}
