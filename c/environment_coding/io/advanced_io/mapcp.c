#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
/*
 *2015年 11月 23日 星期一 20:07:40 CST
 *<nicksimon109445@gmail.com>
 *cp a file through mmap
 */
int main(int argc,char *argv[]){

    int rfd,wfd;
    if(argc!=3){
        printf("Usage: ./mmapcp src dst");
    }
    //打开文件用于读,并映射到内存中
    if((rfd=open(argv[1],O_RDONLY))<0){
        perror("open file for read error\n");
        return -1;
    }
    //读取文件长度
    struct stat buf;
    if(fstat(rfd,&buf)<0){
        perror("stat error\n");
        return -1;
    }
    size_t fsize=buf.st_size;
    char *rbuf;
    if((rbuf=mmap(NULL,fsize,PROT_READ,MAP_PRIVATE,rfd,0))==MAP_FAILED){
        perror("mmap error\n");
        return -1;
    }
    //打开文件用于写,并映射到内存中,O_TRUNC截断文件,如果文件不存在创建文件
    if((wfd=open(argv[2],O_RDWR | O_CREAT | O_TRUNC,0664))<0){
        perror("open file for write error\n");
        return -1;
    }
    //设置文件长度,否则调用mmap时会发送SIGBUS信号
    if(lseek(wfd,fsize-1,SEEK_SET)<0){
        perror("lseek error\n");
        return -1;
    }
    write(wfd,"",1);
    //映射到内存中
    char *wbuf;
    if((wbuf=mmap(NULL,fsize,PROT_WRITE,MAP_SHARED,wfd,0))==MAP_FAILED){
        perror("mmap wbuf error\n");
        return -1;
    }
    memcpy(wbuf,rbuf,fsize);
    if(munmap(rbuf,fsize)<0){
        perror("munmap rbuf error\n");
        return -1;
    }
    if(munmap(wbuf,fsize)<0){
        perror("munmap wbuf error\n");
        return -1;
    }
	return 0;
}
