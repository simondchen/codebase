#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <io_util.h>
/*
 *2015年 11月 18日 星期三 21:09:03 CST
 *<nicksimon109445@gmail.com>
 *io util
 */

int set_fl(int fd,int flag){
    int stat;
    stat=fcntl(fd,F_GETFL);
    if(stat<0){
        perror("fcntl GET error:");
        return -1;
    }
    stat |= flag;
    if(fcntl(fd,F_SETFL,&stat)<0){
        perror("fcntl SET error:");
        return -1;
    }
    return 0;
}

int clr_fl(int fd,int flag){
    int stat;
    stat=fcntl(fd,F_GETFL);
    if(stat<0){
        perror("fcntl GET error:");
        return -1;
    }
    stat &= (~flag);
    if(fcntl(fd,F_SETFL,&stat)<0){
        perror("fcntl SET error:");
        return -1;
    }
    return 0;
}

//这里返回值使用ssize_t,这样才可以返回-1!!! ssize_t --- signed long
ssize_t readn(int fd,char *buf,size_t n){
    size_t left=n;
    ssize_t nread;
    while(left>0){
        if((nread=read(fd,buf,left))<0){
            // 如果一个也没读,返回-1
            if(left==n){
                return -1;
            }else{
                break;
            }
        }else if(nread==0){
            break;
        }else{
            left-=nread;
            buf+=nread;
        }
    }
    return n-left;
}

ssize_t writen(int fd,char *buf,size_t n){
    size_t left=n;
    ssize_t nwrite;
    while(left>0){
        if((nwrite=write(fd,buf,left))<0){
            if(left==n){
                return -1;
            }else{
                break;
            }
        }else if(nwrite==0){
            break;
        }else{
            left-=nwrite;
            buf+=nwrite;
        }
    }
    return n-left;
}
