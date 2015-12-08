#define _POSIX_SOURCE
#include "signal_util.h"
/*
 *2015年 11月 13日 星期五 21:44:29 CST
 *<nicksimon109445@gmail.com>
 *signal util func
 */

//定义静态全局变量
static int flag=0;
static sigset_t newmask,oldmask,zeromask;

void sig_usr(int signo)
{
    flag=1;
}

//相当于初始化
void TELL_WAIT(void)
{
    //初始化信号集
    sigemptyset(&newmask);
    sigemptyset(&oldmask);
    sigemptyset(&zeromask);
    sigaddset(&newmask,SIGUSR1);
    sigaddset(&newmask,SIGUSR2);
    //设置信号处理函数
    if(signal(SIGUSR1,sig_usr)==SIG_ERR){
        perror("signal SIGUSR1 error:");
        exit(-1);
    }
    if(signal(SIGUSR2,sig_usr)==SIG_ERR){
        perror("signal SIGUSR2 error:");
        exit(-1);
    }
    //阻塞信号,从而控制该信号只可以唤醒因调用sigsuspend阻塞的进程,即在sigprocmask和
    //sigsuspend之间是被保护的临界区
    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0){
        perror("sigprocmask error:");
        exit(-1);
    }
}

void TELL_PARENT(pid_t pid){
    kill(pid,SIGUSR2);
}

void WAIT_PARENT(void){
    while(flag==0){
        //设置非阻塞(procmask)和挂起是原子操作 ---> procmask+pause是原子操作
        //对于其他系统调用,如procmask+select,没有现成的系统调用,要自己写
        sigsuspend(&zeromask);
    }
    flag=0;
    //设置非阻塞
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0){
        perror("sigprocmask SIG_SETMASK error:");
        exit(-1);
    }
}

void TELL_CHILD(pid_t pid){
    kill(pid,SIGUSR1);
}

void WAIT_CHILD(void){
    while(flag==0){
        sigsuspend(&zeromask);
    }
    flag=0;
    //设置非阻塞
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0){
        perror("sigprocmask SIG_SETMASK error:");
        exit(-1);
    }
}

void pr_mask(const char *str){
    sigset_t mask;
    sigemptyset(&mask);
    //获取当前进程的信号屏蔽字
    if(sigprocmask(0,NULL,&mask)<0){
        perror("sigprocmask error:");
        exit(-1);
    }
    printf("%s:",str);
    if(sigismember(&mask,SIGINT)) printf("SIGINT,");
    if(sigismember(&mask,SIGALRM)) printf("SIGALRM,");
    if(sigismember(&mask,SIGUSR1)) printf("SIGUSR1,");
    if(sigismember(&mask,SIGQUIT)) printf("SIGQUIT,");
    printf("\n");
    return;
}

void abort(void){
    //POSIX.1规范:abort并不理会进程对SIGABRT信号的阻塞和忽略
    struct sigaction act;
    //获取进程对SIGABRT的当前动作
    sigaction(SIGABRT,NULL,&act);
    if(act.sa_handler==SIG_IGN){
        act.sa_handler=SIG_DFL;
        sigaction(SIGABRT,&act,NULL);
    }
    if(act.sa_handler==SIG_DFL)
        fflush(NULL);
    //否则的话就是进程设置了对SIGABRT的处理函数
    //阻塞所有信号,不阻塞SIGABRT,调用kill发送SIGABRT信号,若kill返回,则说明信号处理函数没有退出进程(exit等)
    sigset_t newmask;
    sigfillset(&newmask);
    sigdelset(&newmask,SIGABRT);
    sigprocmask(SIG_SETMASK,&newmask,NULL);
    kill(getpid(),SIGABRT);

    //如果执行到这里,说明进程捕获信号且信号处理程序没有退出程序,则重置为默认处理函数
    fflush(NULL);
    act.sa_handler=SIG_DFL;
    sigaction(SIGABRT,&act,NULL);
    sigprocmask(SIG_SETMASK,&newmask,NULL);
    kill(getpid(),SIGABRT);
    exit(1);
}

int system(const char *command)
{
    //创建子进程执行command命令,忽略SIGINT和SIGQUIT,阻塞SIGCHILD
    //父进程逻辑
    //设置信号处理 
    int status=0;
    struct sigaction ignore,saveintr,savequit;
    ignore.sa_handler=SIG_IGN;
    if(sigaction(SIGINT,&ignore,&saveintr)<0){
        return -1;
    }
    if(sigaction(SIGQUIT,&ignore,&savequit)<0){
        return -1;
    }
    sigset_t newmask,oldmask;
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGCHLD);
    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
        return -1;

    //创建子进程,执行command
    pid_t pid=fork(); 
    if(pid<0){
        //fork出错
        status=-1;
    }else if(pid==0){
        //子进程
        //恢复信号处理
        sigaction(SIGINT,&saveintr,NULL);
        sigaction(SIGQUIT,&savequit,NULL);
        sigprocmask(SIG_SETMASK,&oldmask,NULL);
        execl("/bin/sh","sh","-c",command,(char *)0);
        _exit(127); //退出子进程
    }else{
        //父进程逻辑
        //等待子进程退出
        while(waitpid(pid,&status,0)<0){
            if(errno!=EINTR){
                //如果不是被中断,则终止,返回错误-1,否则重启waitpid系统调用
                status=-1;
                break;
            }
        }
        //回复信号处理
        if(sigaction(SIGINT,&saveintr,NULL)<0){
            return -1;
        }
        if(sigaction(SIGQUIT,&savequit,NULL)<0){
            return -1;
        }
        if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
            return -1;
    }

    return status;

}
