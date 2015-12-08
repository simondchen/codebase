#include <progress_util.h>
/*
 *2015年 11月 17日 星期二 19:13:41 CST
 *<nicksimon109445@gmail.com>
 *progress utils source file
 */

void pr_exit(int status){
    if(WIFEXITED(status)){
        printf("normal termination,exit status=%d\n",WEXITSTATUS(status));
    }else if(WIFSIGNALED(status)){
        printf("abnormal termination,signal number=%d%s\n",WTERMSIG(status),
        #ifdef WCOREDUMP
            WCOREDUMP(status)?" (core file generated)":"");
        #else
            "");
        #endif
    }else if(WIFSTOPPED(status)){
        printf("child stopped,signal number=%d\n",WSTOPSIG(status));
    }
}

int main(void){
    pid_t pid;
    int status;
    if((pid=fork())<0){
        perror("fork error:");
        return -1;
    }else if(pid==0){
        return 0;
    }
    wait(&status);
    pr_exit(status);
    if((pid=fork())<0){
        perror("fork error:");
        return -1;
    }else if(pid==0){
        abort();
    }
    wait(&status);
    pr_exit(status);
    if((pid=fork())<0){
        perror("fork error:");
        return -1;
    }else if(pid==0){
        int i=1;
        i /= 0;
    }
    wait(&status);
    pr_exit(status);
    return 0;
}
