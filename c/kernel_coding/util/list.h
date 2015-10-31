#ifndef __LIST_H
#define __LIST_H
#include <linux/kernel.h>
//注意这里要使用-I
/*
 *2015-10-07 15:38 by simon
 *实现list_head双向链表
 *
 */

//实现list_entry宏，注意宏的返回值就是最后一个表达式的值，内核实际实现太复杂，先使用写死的版本代替一下
//#define list_entry(ptr,type,member) \
	container_of(ptr,type,member)

struct list_head {
	struct list_head *prev,*next;
};

//因为这里有对齐的影响，所以children和task_struct的地址未必相差sizeof(int),所以取消对齐
struct task_struct {
	int pid;
	struct list_head own;
	struct list_head children;
}__attribute__((packed));

struct task_struct *list_entry(struct list_head *ptr){
	return (struct task_struct *)(((unsigned char *)ptr)-sizeof(int));
}

//#define INIT_LIST_HEAD(list) (list->prev) = (list->next) = NULL

//初始化链表
static inline void INIT_LIST_HEAD(struct list_head *list){
	list->prev = list;
	list->next = list;
}

//判断链表是否为空，空返回1,非空返回0
int list_empty(struct list_head *head){
	if(head->next == head){
		return 1;
	}
	return 0;
	//内核实现的更简洁
	//return head->next == head;
}

//实现在头部添加
static inline void list_add(struct list_head *new,struct list_head *head){
	new->next = head->next;	
	new->prev = head;
	head->next->prev = new;
	head->next = new;
}

//实现在尾部添加
static inline void list_add_tail(struct list_head *new,struct list_head *head){
	new->prev = head->prev;	
	new->next = head;
	head->prev->next = new;
	head->prev = new;
}


#endif
