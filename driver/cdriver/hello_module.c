#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("simon");
MODULE_DESCRIPTION("just a simple Linux module");

static int __init hello_init(void){
	printk(KERN_DEBUG "abc\n");
	return 0;
}

static void __exit hello_exit(void){
	printk(KERN_DEBUG "module removed\n");
}

module_init(hello_init);
module_exit(hello_exit);
