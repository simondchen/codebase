#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/slab.h>

#define TASK_NUM 100
MODULE_LICENSE("GPL");
char kern_buff[1024] = "hello from kernel\n";

struct task_info{
	pid_t pid;
	unsigned char order;
	unsigned char deep;
	struct task_info *next;
};

//实现一个hash表
//要检查一下next指针初始是否为NULL
static struct task_info *task_info[TASK_NUM]={NULL,};
static int nr_tasks = 0;

static release_task_info(void){
	int i;
	struct task_info *ts,*tmp;
	//kfree并不等于将指针设置成空了！！！要手动将指针数组置空
	for(i=0;i<TASK_NUM;i++){
		ts = task_info[i];
		while(ts!=NULL){
			tmp = ts->next;
			kfree(ts);
			ts = tmp;
		}
		task_info[i] = NULL;
	}
}

static void init_task_info(struct task_info *ti,pid_t pid,uint8_t order,uint8_t deep){
	ti->pid = pid;
	ti->order = order;
	ti->deep = deep;
	ti->next = NULL;
}

static void traversal_task(struct task_struct *task,int deep){
	struct task_info *ti = kmalloc(sizeof(struct task_info),GFP_KERNEL);
	if(ti==NULL){
		printk(KERN_ALERT "malloc task_info error");
		return;
	}
	struct task_info *ptr;
	struct task_struct *ts;
	struct list_head *lh;
	init_task_info(ti,task->pid,nr_tasks,deep);
	ptr = task_info[nr_tasks % TASK_NUM];
	//插入链表队头
	ti->next = ptr;
	//这里错了呀，不是给ptr,是给task_info[]呀！！！
	//ptr = ti;
	task_info[nr_tasks % TASK_NUM] = ti;
	//printk(KERN_ALERT "-----%d:%d-----\n",deep,task->pid);
	//遍历children双向链表
	if(!list_empty(&(task->children))){
		for(lh=(task->children).next;lh!=&(task->children);lh=lh->next){
			ts = list_entry(lh,struct task_struct,sibling);
			nr_tasks++;
			traversal_task(ts,deep+1);
		}
	}
}

//文件操作函数实现
static int window_open(struct inode *inode,struct file *filp){
	printk(KERN_ALERT "in open\n");
	//初始化数据,将会通过read传递到用户层,kmalloc的使用
	struct task_struct *task = &init_task;	
	struct task_struct *ts,*first_task;	
	struct list_head *cursor;
	//试一下kfree后，指向该内存的指针是否为NULL
	/*ts = kmalloc(sizeof(struct task_struct),GFP_KERNEL);
	if(ts == NULL){
		printk(KERN_ALERT "kmalloc error\n");
		return -1;
	}
	kfree(ts);
	if(ts!=NULL){
		printk(KERN_ALERT "oh my god!\n");
	}else{
		printk(KERN_ALERT "thankfully\n");
	}*/
	//获取进程1
	if(!list_empty(&(task->children))){
		for(cursor = (task->children).next;cursor!=&(task->children);cursor=cursor->next){
			ts = list_entry(cursor,struct task_struct,sibling);
			//printk(KERN_ALERT "+++++++++++++%d+++++++\n",ts->pid);
			if(ts->pid == 1)
				first_task = ts;
		}
	}
	//printk(KERN_ALERT "++++++%d+++++++",first_task->pid);
	//traversal_task(first_task,0);
	traversal_task(task,0);
	printk(KERN_ALERT "_+_+_+_+%d_+_+_+",nr_tasks);
	/*struct task_info *tss = task_info[1];
	if(tss!=NULL){
		printk(KERN_ALERT "------%d-------",tss->pid);
	}else{
		printk(KERN_ALERT "task_info is NULL,but why\n");
	}*/
	/*struct task_struct *task = &init_task;	
	struct task_struct *first_task=NULL;
	struct list_head *cursor;
	struct task_struct *ts;
	printk(KERN_ALERT "+++++++++++++%d+++++++\n",task->pid);
	if(!list_empty(&(task->children))){
		for(cursor = (task->children).next;cursor!=&(task->children);cursor=cursor->next){
			ts = list_entry(cursor,struct task_struct,sibling);
			printk(KERN_ALERT "+++++++++++++%d+++++++\n",ts->pid);
			if(ts->pid == 0)
				first_task = ts;
		}
	}
	if(first_task!=NULL && !list_empty(&(first_task->children))){
		for(cursor = (first_task->children).next;cursor!=&(first_task->children);cursor=cursor->next){
			ts = list_entry(cursor,struct task_struct,sibling);
			printk(KERN_ALERT "+++++++++++++%d+++++++\n",ts->pid);
		}
	}*/
			
	return 0;
}

static size_t window_read(struct file *filp,void __user *buff,size_t count,loff_t *offp){
	/*int len = 18;
	printk(KERN_ALERT "file offset:%d\n",*offp);
	copy_to_user(buff,kern_buff,len+1);
	*offp += (len+1);
	return len+1;*/
	struct task_info *ts;
	//divide may be dangerous
	//int order = (*offp)/sizeof(struct task_info);
	int order = *offp;
	printk(KERN_ALERT "in read:%d\n",order);
	//全部读完，返回0告诉应用层停止读取
	if(order > nr_tasks){
		return 0;
	}
	ts = task_info[order % TASK_NUM];	
	while(ts!=NULL){
		if(ts->order == order){
			break;
		}
		ts = ts->next;	
	}
	if(ts == NULL){
		printk(KERN_ALERT "this should never happen,the program is with problem,stop reading\n");
		return -1;
	}
	copy_to_user(buff,ts,sizeof(struct task_info));
	*offp += 1;
	return sizeof(struct task_info);
}

static void window_release(struct inode *inode,struct file *flip){
	//release task_info hash table
	release_task_info();	
}

//声明全局变量
int major;
struct cdev dev;
struct class *window_class;

struct file_operations window_ops = {
	.owner = THIS_MODULE,	
	.open = window_open,
	.read = window_read,
	.release = window_release
};
//做好出错处理

static int window_init(void){
	
	dev_t devno;
	int err;
	printk(KERN_ALERT "init\n");
	//1.申请设备号
	//这里alloc_chrdev_region的返回值怎么规定的???
	if((err=alloc_chrdev_region(&devno,0,1,"window"))){
		printk(KERN_ALERT "alloc chrdev region error\n");
		goto alloc_fail;
	}
	major = MAJOR(devno);	

	//2.注册字符设备到内核
	cdev_init(&dev,&window_ops);
	dev.owner = THIS_MODULE;
	dev.ops = &window_ops;
	//上次吃了cdev_add的亏，第三个参数比表示添加的设备的个数
	if((err=cdev_add(&dev,devno,1))){
		printk(KERN_ALERT "cdev add error\n");
		goto cdev_add_fail;
	}
	//3.创建设备文件
	if(!(window_class=class_create(THIS_MODULE,"window"))){
		printk(KERN_ALERT "create class error\n");
		goto class_create_fail;
	}
	if(!(device_create(window_class,NULL,devno,NULL,"window",0))){
		printk(KERN_ALERT "create device error\n");
		goto device_create_fail;
	}

	return 0;
	
	device_create_fail:
		class_destroy(window_class);
	class_create_fail:
		cdev_del(&dev);
	cdev_add_fail:	
		//释放设备号
		unregister_chrdev_region(MKDEV(major,0),1);
	alloc_fail:
	return -1;
}

static void window_exit(void){
	//退出时释放资源
	printk(KERN_ALERT "exit\n");
	//释放链表分配的内存
	//1.释放device
	release_task_info();	
	device_destroy(window_class,MKDEV(major,0));
	//2.释放类文件
	class_destroy(window_class);
	//3.删除字符设备
	cdev_del(&dev);
	//4.释放设备号
	unregister_chrdev_region(MKDEV(major,0),1);
}

module_init(window_init);
module_exit(window_exit);
