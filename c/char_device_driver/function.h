#ifndef __FUNCTION_H
#define __FUNCTION_H

#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/fs.h>
typedef struct list_head list_head;
typedef struct task_struct task_struct;


void printTree(task_struct *start, int *pre, int prenum , int offset);
void printThread(void);
void printMemstat(int pid);
void printMoreInfo(int pid);
extern char globalBuff[];
#endif
