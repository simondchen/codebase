#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
/*
 *2015年 11月 10日 星期二 22:46:28 CST
 *<nicksimon109445@gmail.com>
 *walk through a dir and record total numbers of different type of files
 */
#define NR_TYPES 7
enum FILE_TYPE{REG,DIRE,BLK,CHR,LNK,SOCK,FIFO};

//全局数组,用于统计各种类型的文件的数目
int file_type[NR_TYPES];

typedef int (*entry_handle)(char *path);

void checktype(struct stat st){
    mode_t mode=st.st_mode;
    if(S_ISREG(mode)){
        file_type[REG]++;
        return;
    }
    if(S_ISDIR(mode)){
        file_type[DIRE]++;
        return;
    }
    if(S_ISBLK(mode)){
        file_type[BLK]++;
        return;
    }
    if(S_ISCHR(mode)){
        file_type[CHR]++;
        return;
    }
    if(S_ISLNK(mode)){
        file_type[LNK]++;
        return;
    }
    if(S_ISFIFO(mode)){
        file_type[FIFO]++;
        return;
    }
    if(S_ISSOCK(mode)){
        file_type[SOCK]++;
        return;
    }
}

int handle(char *path){
    struct stat st;
    printf("%s\n",path);
    if(stat(path,&st)<0){
        perror("stat error\n");
        return -1;
    }
    checktype(st);
    return 0;
}

int ftw(char *path, entry_handle handle){
    //这里要注意的是要维护路径!!!
    //1.处理树根
    //首先判断是不是目录,不是目录则退出
    struct stat buf;
    if(stat(path,&buf)<0){
        perror("stat error\n");
        return -1;
    }
    if(!S_ISDIR(buf.st_mode))
        return 0;
    //这里树根就不要再调用handle,否则在其父节点又会调用一次,重复
    /*
    if(handle(path)<0)
        return -1;
    */
    //2.获取第一个DIR stream
    DIR *dir;
    struct dirent *entry;
    dir=opendir(path);
    if(dir==NULL){
        perror("opendir error\n");
        return -1;
    }
    //3.遍历dir的各个entry,注意去掉.和..
    entry=readdir(dir);
    while(entry!=NULL){
        if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0){
            entry=readdir(dir);
            continue;
        }
        char entry_path[1024];
        sprintf(entry_path,"%s/%s",path,entry->d_name);
        handle(entry_path);
        if(ftw(entry_path,handle)<0)
            return -1;
        entry=readdir(dir);
    }
    closedir(dir);
    return 0;
}

int main(int argc,char *argv[]){
    if(ftw(argv[1],handle)<0){
        printf("ftw error\n");
        return -1;
    }
    int i;
    for(i=0;i<NR_TYPES;i++){
        printf("%d\n",file_type[i]);
    }
    printf("%d\n",REG);
}
