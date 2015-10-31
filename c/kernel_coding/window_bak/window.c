#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");

//文件操作函数实现
static int window_open(struct inode *inode,struct file *filp){
	printk(KERN_ALERT "in open\n");
	return 0;
}

//声明全局变量
int major;
struct cdev dev;
struct class *window_class;

struct file_operations window_ops = {
	.owner = THIS_MODULE,	
	.open = window_open
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
	//1.释放device
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
