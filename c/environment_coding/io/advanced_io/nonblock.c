#include <stdio.h>
#include <unistd.h>
#include <io_util.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
/*
 *2015年 11月 18日 星期三 20:42:23 CST
 *<nicksimon109445@gmail.com>
 *practice nonblock io
 */
int main(int argc,char *argv[]){

    char buf[500000];    
    int ntowrite=read(STDIN_FILENO,buf,sizeof(buf));
    if(ntowrite<0){
        perror("read error:");
        return -1;
    }
    fprintf(stderr,"read %d bytes\n",ntowrite);
    //设置非阻塞
    if(set_fl(STDOUT_FILENO,O_NONBLOCK)<0)
        return -1;

    int writenum=0;
    char *ptr=buf;
    while(ntowrite>0){
       writenum=write(STDOUT_FILENO,ptr,ntowrite); 
       fprintf(stderr,"writenum=%d,errno=%d\n",writenum,errno);
       if(writenum>0){
           ptr+=writenum;
           ntowrite-=writenum;
       }
    }

    //清除flag
    if(clr_fl(STDOUT_FILENO,O_NONBLOCK)<0)
        return -1;

	return 0;
}
