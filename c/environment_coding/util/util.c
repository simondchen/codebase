#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <string.h>
/*
 *2015年 11月 08日 星期日 19:42:46 CST
 *<nicksimon109445@gmail.com>
 *practical func
 */

int close_all_fds(void){
    //获取进程可打开文件描述符的最大值ulimit -n
    struct rlimit limit;
    long maxfd;
    memset(&limit,0,sizeof(limit));
    if(getrlimit(RLIMIT_NOFILE,&limit)<0)
        perror("getrlimit error:");
    //a value one greater than the max fd ...
    maxfd=limit.rlim_cur;
    int i;
    for(i=0;i<maxfd;i++){
        close(i);
    }
    return 0;
}

int daemon(int nochdir,int noclose){
    pid_t pid,s_pid;
    //1.fork()创建子进程,关闭父进程
    pid=fork();
    if(pid<0){
        perror("fork error\n");
        return -1;
    }
    if(pid>0){
        //退出父进程,这里不可以使用return,它只是返回daemon函数
        exit(1);
    }
    //子进程逻辑
    //2.新建会话组以脱离之前会话组的控制(shell)
    s_pid=setsid();
    if(s_pid<0){
        perror("setsid error\n");
        return -1;
    }
    //3.如果设置了nochdir,则不切换进程的根目录
    if(!nochdir){
        if(chdir("/")<0){
            perror("change the working directory of the calling process failed\n");
            return -1;
        }
    }
    if(!noclose){
        //将0,1,2定向到/dev/null
        //关闭标准输入,并重新定向到/dev/null
        close(0);
        close(1);
        close(2);
        open("/dev/null",O_RDWR);
        dup(0);
        dup(0);
    }
    return 0;

}

int main(int argc,char *argv[]){
    if(open("/dev/ttyS0",O_RDONLY)<0)
        perror("open serial port error\n");
    sleep(30);
    close_all_fds();
	return 0;
}
