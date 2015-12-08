#include <sys/types.h>
int set_fl(int fd,int flag);
int clr_fl(int fd,int flag);
ssize_t readn(int fd,char *buf,size_t n);
ssize_t writen(int fd,char *buf,size_t n);
