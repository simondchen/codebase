#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

const char *space = "|    ";
const char *line = "|___ ";
struct task_struct init_task;
struct task_struct *tss[3];

//在堆中分配的内存要记得释放，否则会造成内存泄漏,这样交由外部去释放的做法不好，一会改一下
char *construct_string(int deep){
	char *str = malloc((5*deep+1)*sizeof(char));
	int i;
	if(str == NULL){
		return NULL;
	}
	if(deep == 0){	
		str[0] = '\0';
	}
	else{
		for(i=0;i<deep-1;i++){
			strncat(str+i*5,space,5*sizeof(char));
		}
		strcat(str+i*5,line);
	}
	return str;
}

void task_print_tree(struct task_struct *task,int deep){
	char *str = construct_string(deep);	
	struct list_head *ptr;
	struct task_struct *ts;
	printf("%s%d\n",str,task->pid);
	//释放堆中分配的内存
	free(str);
	//子进程链不是空的,循环
	if(!list_empty(&(task->children))){
		for(ptr=(task->children).next; ptr!=&(task->children); ptr=ptr->next){	
			ts = list_entry(ptr);
			task_print_tree(ts,deep+1);
		}
	}
}

void task_init(struct task_struct *task,int pid){
	task->pid = pid;	
	INIT_LIST_HEAD(&(task->own));
	INIT_LIST_HEAD(&(task->children));
}

int init(void){
	task_init(&init_task,0);
	//添加进程1
	tss[0] = malloc(sizeof(struct task_struct));
	if(tss[0] == NULL){
		printf("malloc error\n");
		return -1;
	}
	task_init(tss[0],1);
	list_add_tail(&(tss[0]->own),&(init_task.children));
	//添加进程2
	tss[1] = malloc(sizeof(struct task_struct));
	if(tss[1] == NULL){
		printf("malloc error\n");
		return -1;
	}
	task_init(tss[1],2);
	list_add_tail(&(tss[1]->own),&(init_task.children));
	//添加进程3
	tss[2] = malloc(sizeof(struct task_struct));
	if(tss[2] == NULL){
		printf("malloc error\n");
		return -1;
	}
	task_init(tss[2],3);
	list_add_tail(&(tss[2]->own),&(tss[0]->children));
	return 0;
};

int main(void){
	struct task_struct *ts;
	struct list_head *ptr;
	if(init()<0){
		printf("init error\n");
		return -1;
	}

	/*for(ptr=(init_task.children).next;ptr!=&(init_task.children);ptr=ptr->next){
		ts = list_entry(ptr);
		printf("%d\n",ts->pid);
	}
	for(ptr=(tss[0]->children).next;ptr!=&(tss[0]->children);ptr=ptr->next){
		ts = list_entry(ptr);
		printf("%d\n",ts->pid);
	}*/
	task_print_tree(&init_task,0);

	//释放，防止内存泄漏
	free(tss[0]);
	free(tss[1]);
	free(tss[2]);
	return 0;
}
