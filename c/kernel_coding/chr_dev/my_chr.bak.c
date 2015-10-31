#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include "my_chr.h"


MODULE_LICENSE("GPL");

int my_chr_open(struct inode *inode,struct file *file){
	printk(KERN_ALERT "in open\n");
	return 0;
}

size_t my_chr_read(struct file *file,char __user *buff,size_t len,loff_t *off){
	return 0;
}

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = my_chr_open,
	.read = my_chr_read,
};

static int my_chr_major = MY_CHR_MAJOR;
struct my_chr_cdev my_dev;
dev_t devno;
int dev_nr = 1;

static void my_chr_init(void){
	//1.分配设备号
	int result;
	if(my_chr_major){
		devno = MKDEV(my_chr_major,0);
		result = register_chrdev_region(devno,dev_nr,"my_chr");
	}
	else{
		result = alloc_chrdev_region(&devno,0,dev_nr,"my_chr");
		my_chr_major = MAJOR(result);
	}
	if(result < 0){
		printk(KERN_ALERT "alloc device number fail\n");
	}
	
	//2.注册字符设备到内核
	cdev_init(&(my_dev.dev),&f_ops);
	my_dev.dev.owner = THIS_MODULE;
	my_dev.dev.ops = &f_ops;
	cdev_add(&(my_dev.dev),devno,0);
	
}

static void my_chr_exit(void){
	cdev_del(&(my_dev.dev));
	unregister_chrdev_region(devno,dev_nr);
	printk(KERN_ALERT "module exit\n");
}

module_init(my_chr_init);
module_exit(my_chr_exit);
