#include <stdio.h>
#include <setjmp.h>
/*
 *2015年 11月 03日 星期二 10:27:58 CST
 *<nicksimon109445@gmail.com>
 *utilize setjmp/longjmp to jmp from low hierarchy to high hierarchy func
 */

//定义jmp_buf全局变量,用于保存函数上下文
jmp_buf jmpbuf;

//定义两个函数用于实验
void func1(int *a){
    printf("in func1\n");
    //跳转到high hierarchy func,即跨层次的函数跳转,而像return是返回到上一层函数
    if(a==NULL){
        longjmp(jmpbuf,1);
    }
    printf("pointer not null\n");
}

void func2(int *a){
    printf("in func1\n");
    //跳转到high hierarchy func
    if(a==NULL){
        longjmp(jmpbuf,2);
    }
    printf("pointer not null\n");
}

int main(int argc,char *argv[]){
    //设置跳转点
    int i;
    int b[1];
    i=setjmp(jmpbuf);             
    if(i==0){
        //正常处理流程
        //call setjmp directly
        func1(b);
        //这里函数2应该执行不到吧???
        func2(NULL);
    }else{
        //底层函数出错时的异常处理
        //jmp with longjmp from low hierarchy func
        if(i==1){
            printf("error occurs in func1,handle the exception\n");
        }
        if(i==2){
            printf("error occurs in func2,handle the exception\n");
        }
    }
    return 0;
}
