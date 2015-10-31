#include "function.h"

void printTree(task_struct *start, int *pre, int prenum , int offset){
    task_struct * pcb = start;
	list_head *c = &(start ->children);
	char buf[10];
	sprintf(buf,"%5d",pcb->pid);
    strcat(globalBuff,buf);
	int namelen = strlen(buf);
	memset(buf,'\0',10);
    if(c->next != c && c->next->next == c){
		strcat(globalBuff,"───");
        printTree(list_entry(c->next, task_struct, sibling), pre, prenum, offset + namelen + 3);
    }else if(!list_empty(c)){
		list_head *it;
		list_for_each(it, c){
        int *t = (int*)kmalloc(sizeof(int)*(prenum+1), GFP_KERNEL);
        int i;
		for( i = 0; i < prenum; i ++)
            t[i] = pre[i];
        t[prenum] = offset + namelen + 1;
        if(it == c ->next){
			strcat(globalBuff,"─┬─");
            printTree(list_entry(it, task_struct, sibling), t, prenum + 1, offset + namelen + 3);
        }else if(it != c->prev){
            int index = 0;
			int k;
            for( k = 0; k < offset + namelen; k++){
                if(index < prenum && k == pre[index]){
					strcat(globalBuff,"│");
                    index ++;
                }else strcat(globalBuff," ");
            }
			strcat(globalBuff," ├─");
            printTree(list_entry(it, task_struct, sibling), t, prenum + 1, offset + namelen + 3);
        }else{
            int index = 0;
            int m ;
            for(m = 0; m < offset + namelen; m++){
                    if(index < prenum && m == pre[index]){
                        strcat(globalBuff,"│");
                        index ++;
                    }else strcat(globalBuff," " );
                }     
                strcat(globalBuff, " └─");
                printTree(list_entry(it, task_struct, sibling), pre, prenum, offset + namelen + 3);
            }
            if(t != NULL){
                kfree(t);
                t = NULL;
            }            
        }
    }else{
		strcat(globalBuff,"\n");
    }
}



void printThread(void)
{
    task_struct *task,*thread;
    char idbuf[100];
    do_each_thread(task,thread)
    {
        memset(idbuf,'\0',100);
        if((int)thread->pid == (int)thread->tgid)
        {
            printk("in if thread->pid == thread->tgid = %5d\n",thread->tgid);
            sprintf(idbuf,"\nthread group %5d contains thread:",thread->pid);
            strcat(globalBuff,idbuf);
            memset(idbuf,'\0',100);
        }
        sprintf(idbuf,"%5d",thread->pid);
        strcat(globalBuff,idbuf);
    }
     while_each_thread(task,thread);
}


void printMemstat(int pid)
{
    char buf[100];
    struct vm_area_struct *vma;
    task_struct *task = pid_task(find_vpid(pid),PIDTYPE_PID);
    struct mm_struct *mm = task->mm;
    sprintf(buf,"start_code :%p,end_code :%p,size:%d\n",mm->start_code,mm->end_code,mm->end_code - mm->start_code);
    strcat(globalBuff,buf);
    sprintf(buf,"start_data :%p,end_data :%p,size:%d\n",mm->start_data,mm->end_data,mm->end_data - mm->start_data);
    strcat(globalBuff,buf);
    sprintf(buf,"start_heap :%p,end_heap :%p,size:%d\n",mm->start_brk,mm->brk,mm->brk - mm->start_brk);
    strcat(globalBuff,buf);
    sprintf(buf,"start_stack:%p,end_stack:%p,size:%d\n",mm->start_stack,mm->start_stack - mm->stack_vm * 4096,mm->stack_vm * 4096);
    strcat(globalBuff,buf);
    sprintf(buf,"page directory :%p\n",mm->pgd);
    strcat(globalBuff,buf);
    sprintf(buf,"all physical memory:%d frames\n",atomic_read(&mm->rss_stat.count[MM_FILEPAGES])+atomic_read(&mm->rss_stat.count[MM_ANONPAGES]));
    strcat(globalBuff,buf);
    sprintf(buf,"there are %d vma:\n",mm->map_count);
    strcat(globalBuff,buf);

    down_read(&mm->mmap_sem);
    for(vma = mm->mmap;vma;vma = vma->vm_next)
    {
        sprintf(buf,"  %p------%p:",vma->vm_start,vma->vm_end);
        strcat(globalBuff,buf);
        if(vma->vm_flags & VM_WRITE)
        {
            sprintf(buf,"WRITE ");
            strcat(globalBuff,buf);
        }
        if(vma->vm_flags & VM_READ)
        {
            sprintf(buf,"READ ");
            strcat(globalBuff,buf);
        }
        if(vma->vm_flags & VM_EXEC)
        {
            sprintf(buf,"EXEC");
            strcat(globalBuff,buf);
        }
        sprintf(buf,"\n");
        strcat(globalBuff,buf);
    }
    up_read(&mm->mmap_sem);
}


static const char *task_state_array[]={
    "R(running)",
    "S(sleeping)",
    "D(disk sleep)",
    "T(stoped)",
    "T(tracing stop)",
    "Z(zombie)",
    "X(dead)"
};


static inline const char *get_task_state(struct task_struct *tsk)
{
    unsigned int state = (tsk->state & TASK_REPORT) | tsk->exit_state;
    const char **p = &task_state_array[0];
    while(state)
    {
        p++;
        state >>= 1;
    }
    return *p;
}


void printMoreInfo(int pid)
{
    char buf[100];
    struct vm_area_struct *vma;
    task_struct *task = pid_task(find_vpid(pid),PIDTYPE_PID);
    struct mm_struct *mm = current->mm;

    char *state = get_task_state(task);
    strcat(globalBuff,"status:");
    strcat(globalBuff,state);
    strcat(globalBuff,"\n");
    sprintf(buf,"user:%d,group:%d\n",task->cred->uid,task->cred->gid);
    strcat(globalBuff,buf);
    sprintf(buf,"prio:%d\n",task->prio);
    strcat(globalBuff,buf);
    sprintf(buf,"mmap_base:%p\n",mm->mmap_base);
    strcat(globalBuff,buf);
    sprintf(buf,"shared vm:%d frames\n",mm->shared_vm);
    strcat(globalBuff,buf);
    sprintf(buf,"arg_start:%p,arg_end:%p\n",mm->arg_start,mm->arg_end);
    strcat(globalBuff,buf);
    sprintf(buf,"env_start:%p,env_end:%p\n",mm->env_start,mm->env_end);
    strcat(globalBuff,buf);
}
