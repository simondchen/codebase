#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#define MY_CHR_MAJOR 0

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
	.read = my_chr_read
};

int my_chr_major = MY_CHR_MAJOR;
struct cdev dev;
int dev_nr = 1;
struct class *my_class;

static void my_chr_init(void){
	//1.分配设备号
	int result;
	int errno;
	dev_t devno = MKDEV(my_chr_major,0);
	result = alloc_chrdev_region(&devno,0,dev_nr,"my_chr");
	if(result > 0){
		printk(KERN_ALERT "alloc device number fail\n");
		return;
	}
	my_chr_major = MAJOR(result);
	
	//2.注册字符设备到内核
	cdev_init(&dev,&f_ops);
	dev.owner = THIS_MODULE;
	dev.ops = &f_ops;
	errno=cdev_add(&dev,devno,1);
	if(errno){
		printk(KERN_ALERT "add chrdev error\n");
	}

	//3.创建文件设备节点
	my_class = class_create(THIS_MODULE,"my_chr");
	if(!my_class){
		printk(KERN_ALERT "create class error\n");
		return;	
	}

	if(!(device_create(my_class,NULL,devno,NULL,"my_chr",0))){
		printk(KERN_ALERT "create device error\n");
		return;
	}
}

static void my_chr_exit(void){
	device_destroy(my_class,MKDEV(my_chr_major,0));
	class_destroy(my_class);
	cdev_del(&dev);
	unregister_chrdev_region(MKDEV(my_chr_major,0),dev_nr);
	printk(KERN_ALERT "module exit\n");
}

module_init(my_chr_init);
module_exit(my_chr_exit);
