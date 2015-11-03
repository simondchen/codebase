#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
/*
 *2015年 11月 03日 星期二 09:07:40 CST
 *<nicksimon109445@gmail.com>
 *set the jmp_buf directly without calling setjmp to implement custom jmp
 */

//可以这样定义,相当于两块连续的struct __jmp_buf_tag结构体
sigjmp_buf buf[2];
//定义两块栈
uint8 stack[2][1024];

void switch_routine(int sig){
}

void routine_a(){
}

void routine_b(){
}

int create_thread(void){
    //填充jmp_buf
     
    return 0;
}

int main(int argc,char *argv[]){
    (*buf[0]).__jmpbuf[JB_PC]=routine_a;
    (*buf[0]).__jmpbuf[JB_SP]=stack[0];
	return 0;
}
