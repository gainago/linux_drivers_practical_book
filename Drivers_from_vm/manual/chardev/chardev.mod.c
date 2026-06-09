#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x40c97d4d, "try_module_get" },
	{ 0xa5cca762, "__register_chrdev" },
	{ 0x69b07fa1, "class_create" },
	{ 0xe4e6be1f, "device_create" },
	{ 0x37456001, "device_destroy" },
	{ 0x73ca058d, "class_destroy" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xc3aaf0a9, "__put_user_1" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x92997ed8, "_printk" },
	{ 0x56c60877, "module_put" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");

