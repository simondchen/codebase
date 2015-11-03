#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
/*
 *2015年 11月 02日 星期一 21:21:36 CST
 *<nicksimon109445@gmail.com>
 *epoll practice
 */

//把出错处理封装以下,不然每次都写太麻烦
int setnonblock(int fd,int block){
    //block为1设置成非阻塞,为0设置为阻塞
    int flags;
    flags=fcntl(fd,F_GETFL);
    if(flags<0){
        perror("fcntl get flags error\n");
        return -1;
    }
    if(block){
        flags |= O_NONBLOCK;
        if(fcntl(fd,F_SETFL,flags)<0){
            perror("fcntl set flags error\n");
            return -1;
        }
    }else{
        flags &= ~O_NONBLOCK;
        if(fcntl(fd,F_SETFL,flags)<0){
            perror("fcntl set flags error\n");
            return -1;
        }
    }
    return 0;
}

int main(int argc,char *argv[]){

    //设置服务器地址
    struct sockaddr_in serv;
    memset(&serv,0,sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(9888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //创建套接字
    int fd; 
    if((fd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket error\n");
        return -1;
    }
    //绑定地址
    if(bind(fd,(struct sockaddr *)&serv,sizeof(serv))<0){
        perror("bind error\n");
        close(fd);
        return -1;
    }
    //设置监听
    if(listen(fd,10)<0){
        perror("listen error\n");
        close(fd);
        return -1;
    }

    //下面是epoll的处理逻辑了!!! 
    //创建epoll set
    int eset;
    eset=epoll_create(10);
    if(eset<0){
        perror("epoll_create error\n");
        close(fd);
        return -1;
    }
    //为accept创建epoll_event监听数据到来
    struct epoll_event events[10];
    memset(events,0,10*sizeof(struct epoll_event));
    struct epoll_event revent;
    revent.events = EPOLLIN | EPOLLET;
    //这里添加event的时候需要设置其中的fd项来标识是哪一个socket
    revent.data.fd = fd;
    epoll_ctl(eset,EPOLL_CTL_ADD,fd,&revent);

    while(1){
        int fds,i;
        int cd;
        //注意这里events是出参,传递内存地址,有函数填充
        fds=epoll_wait(eset,events,10,-1);
        if(fds<0){
            perror("epoll_wait error\n");
            close(fd);
            close(eset);
            return -1;
        }
        printf("epoll_wait ret,new event comes\n");
        for(i=0;i<fds;i++){
            //首先判断是否是新的连接到来
            printf("in for loop:%d\n",i);
            if(events[i].data.fd == fd){
                //说明有新的连接到来,调用accept获取该连接的描述符
                printf("new connection comes\n");
                cd=accept(fd,NULL,NULL);   
                if(cd<0){
                    perror("cd error\n");
                    return -1;
                }
                //为新的socket描述符创建event,并将它添加到eset中
                //使用edge trigger,则将socket设置成nonblock
                if(setnonblock(cd,1)<0){
                    return -1;
                }
                struct epoll_event cevent;
                cevent.events = EPOLLIN | EPOLLET;
                cevent.data.fd = cd;
                epoll_ctl(eset,EPOLL_CTL_ADD,cd,&cevent);
            }else{
                //根据epoll_event的类型做相应处理
                int type = events[i].events;
                if(type==EPOLLIN){
                    //数据到来,读取数据,如果有数据要发送,构造event,并注册到cset中
                    printf("data comes\n");
                    //读取数据的方式不正确,应该是利用fd读取数据吧
                    //char *buf=events[i].data.ptr;
                    //printf("%s",buf);
                    char buf[1024];
                    int size;
                    int rcd=events[i].data.fd;
                    size=read(rcd,buf,1024);
                    if(size<0){
                        printf("read error\n");
                        goto release;
                    }
                    printf("%s",buf);
                }
            }
        }
    }
release:
    //释放资源,这里释放资源应该不是这么简单,应该释放eset中的所有event
    close(fd);
    close(eset);
	return 0;
}
