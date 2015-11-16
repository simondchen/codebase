#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
/*
 *2015年 11月 10日 星期二 21:32:44 CST
 *<nicksimon109445@gmail.com>
 *check file stat
 */

int main(int argc,char *argv[]){
    struct stat st;
    int fd;
    mode_t mode;
    if((fd=open("./a",O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP))<0){
        perror("open file error\n");
        return -1;
    }
    fstat(fd,&st);
    mode=st.st_mode;
    if(!(mode & S_IXOTH)){
        printf("file not exculatable\n");
    }
	return 0;
}
