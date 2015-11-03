#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <errno.h>
/*
 *2015年 11月 02日 星期一 23:13:44 CST
 *<nicksimon109445@gmail.com>
 *epoll practise
 */
int main(int argc,char *argv[]){
    //设置地址
    struct sockaddr_in serv;
    bzero(&serv,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(9888);
    serv.sin_addr.s_addr=inet_addr("127.0.0.1");
    //创建套接字
    int fd;
    if((fd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket error\n");
        return -1;
    }
    //连接服务器
    if(connect(fd,(struct sockaddr *)&serv,sizeof(serv))<0){
        perror("connect error\n");
        close(fd);
        return -1;
    }
    //发送数据
    char buf[1024]="hello serv";
    int len; 
    strcat(buf,argv[1]);
    strcat(buf,"\n");
    while(1){
        sleep(1);
        len=send(fd,buf,strlen(buf)+1,0);
        if(len<0){
            perror("send error\n");
            return -1;
        }
    }
    return 0;
}
