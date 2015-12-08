#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

static int pfd1[2],pfd2[2];

void TELL_WAIT(void)
{
    if(pipe(pfd1)<0 || pipe(pfd2)<0){
        perror("pipe error");
        exit(-1);
    }
}

void
TELL_PARENT(void)
{
    if(write(pfd2[1],"c",1)<0){
        perror("TELL_WAIT write error");
        exit(-1);
    }
}

void
WAIT_PARENT(void)
{
    char c;
    if(read(pfd1[0],&c,1)<0){
        perror("WAIT_PARENT read error");
        exit(-1);
    }
    if(c!='c'){
        printf("WAIT_PARENT incorrect data\n");
        exit(-1);
    }
}

void
TELL_CHILD(void)
{
    if(write(pfd1[1],"c",1)<0){
        perror("TELL_WAIT write error");
        exit(-1);
    }
}

void
WAIT_CHILD(void)
{
    char c;
    if(read(pfd2[0],&c,1)<0){
        perror("WAIT_PARENT read error");
        exit(-1);
    }
    if(c!='c'){
        printf("WAIT_PARENT incorrect data\n");
        exit(-1);
