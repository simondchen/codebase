#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/device.h>

#include "memdev.h"
#include "function.h"

MODULE_LICENSE("GPL");
static struct cdev char_device;
static int memdev_major;
static struct class *memdev_class;

char globalBuff[8192 * 10];


int char_device_open(struct inode *inode,struct file *filp)
{
	return 0;
}


int char_device_release(struct inode *inode,struct file *filp)
{
	return 0;
}


ssize_t char_device_read(struct file *filp,char __user *buf,size_t size,loff_t *ppos)
{
	return -ENOSYS;
}


long char_device_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int err = 0;
    int ret = 0;
  
   	if (_IOC_TYPE(cmd) != CHARDEV_IOC_MAGIC) 
        return -EINVAL;
    if (_IOC_NR(cmd) > 4) 
        return -EINVAL;
	
    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
    if (err) 
        return -EFAULT;
 
    switch(cmd) {
	    case CHARDEV_PSTREE:
	    	printk("<--- CMD CHARDEV_PSTREE Begin--->\n\n");
			memset(globalBuff,'\0',81920);
			printTree(&init_task, NULL, 0 , 0);
			copy_to_user((struct type1_data*)arg,globalBuff,81920);
	        printk("<--- CMD CHARDEV_PSTREE Done--->\n\n");
	        break;
	    case CHARDEV_THREADGRP:
	    	printk("<--- CMD CHARDEV_THREADGRP Begin--->\n\n");
	      	memset(globalBuff,'\0',81920);
	      	printThread();
	      	copy_to_user((struct type1_data*)arg,globalBuff,81920);
	  		printk("<--- CMD CHARDEV_THREADGRP Done--->\n\n");
	  		break;
	  	case CHARDEV_MEMSTAT:
	    	printk("<--- CMD CHARDEV_MEMSTAT Begin--->\n\n");
	  		memset(globalBuff,'\0',81920);
	  		struct type2_data *pdata = (struct type2_data*)arg;
	  		printMemstat(pdata->pid);
	  		copy_to_user((char*)(struct type2_data*)pdata->data,globalBuff,81920);
	  		printk("<--- CMD CHARDEV_MEMSTAT Done--->\n\n");
	  		break;
	  	case CHARDEV_PROCDETAIL:
	    	printk("<--- CMD CHARDEV_PROCDETAIL Begin--->\n\n");
	  		memset(globalBuff,'\0',81920);
	  		struct type2_data *p = (struct type2_data*)arg;
	  		printMoreInfo(p->pid);
	  		copy_to_user((char*)(struct type2_data*)p->data,globalBuff,81920);
	  		printk("<--- CMD CHARDEV_PROCDETAIL Done--->\n\n");
	  		break;
	    default:  
	        return -EINVAL;
	    }
    return ret;
}


ssize_t char_device_write(struct file *filp,const char  __user *buf,size_t size,loff_t *ppos)
{
	return -ENOSYS;
}


static const struct file_operations char_device_ops = 
{
	.owner = THIS_MODULE,
	.read = char_device_read,
	.write = char_device_write,
	.open = char_device_open,
	.release = char_device_release,
	.unlocked_ioctl = char_device_unlocked_ioctl
};


static int mem_dev_init(void)
{
	int err = -ENOMEM;
	dev_t mjr = MKDEV(memdev_major,0);
	
	if((err = alloc_chrdev_region(&mjr,0,1,"memdev")))
	{
		printk("alloc_chrdev_region error\n");
		goto fail_register;
	}

	memdev_major = MAJOR(mjr);
	printk(KERN_ALERT "Device number is %d\n",mjr);
	printk(KERN_ALERT "Major number is %d\n",MAJOR(mjr));
	printk(KERN_ALERT "Minor number is %d\n",MINOR(mjr));

	cdev_init(&char_device,&char_device_ops);
	err = cdev_add(&char_device,mjr,MAX_MINOR_NUM);
	if(err)
	{
		printk(KERN_ALERT "cdev_add fail\n");
		goto fail_cdev_add;
	}

	if(!(memdev_class = class_create(THIS_MODULE,"memdev")))
	{
		printk("class_create error\n");
		goto fail_class_create;
	}

	if(!(device_create(memdev_class,NULL,MKDEV(memdev_major,0),NULL,"memdev",0)))
	{
		printk("device_create error\n");
		goto fail_device_create;
	};
	return 0;

	fail_device_create:
		class_destroy(memdev_class);
	fail_class_create:
		cdev_del(&char_device);
	fail_cdev_add:
		unregister_chrdev_region(mjr,MAX_MINOR_NUM);
	fail_register:
		return 0;
}


static void mem_dev_exit(void)
{
	printk(KERN_ALERT "Over\n");
	device_destroy(memdev_class,MKDEV(memdev_major,0));
	class_destroy(memdev_class);
	cdev_del(&char_device);
	unregister_chrdev_region(MKDEV(memdev_major,0),MAX_MINOR_NUM); 
}

module_init(mem_dev_init);
module_exit(mem_dev_exit);
