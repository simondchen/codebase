#include <linux/module.h>
#include <linux/init.h>
#include <linux/time.h>

MODULE_LICENSE("GPL");

static void hello_init(void){
	struct timeval val;
	printk(KERN_ALERT "enter hello world\n");
	jiffies_to_timeval(1000,&val);	
	printk(KERN_ALERT "%d:%d\n",val.tv_sec,val.tv_usec);
}

static void hello_exit(void){
	printk(KERN_ALERT "exit hello world\n");
}

module_init(hello_init);
module_exit(hello_exit);
