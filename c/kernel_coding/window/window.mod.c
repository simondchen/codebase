#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x2cd57c74, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xc74666d8, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xb52d9ea0, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xff2d640f, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x97fbae37, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x78691fc1, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xf9d0d5c0, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xde3dcc89, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xe2b62998, __VMLINUX_SYMBOL_STR(init_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x31c659a2, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xbf92416a, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "265FE0AB50DD7970A0CE5BB");
