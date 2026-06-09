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
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0x37e16a57, "cdev_del" },
	{ 0x37a0cba, "kfree" },
	{ 0xc4aa18f, "kmalloc_caches" },
	{ 0xd8ba0fc0, "__kmalloc_cache_noprof" },
	{ 0x8d6aff89, "__put_user_nocheck_4" },
	{ 0xfe8c61f0, "_raw_read_lock" },
	{ 0xdd4d55b6, "_raw_read_unlock" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xe68efe41, "_raw_write_lock" },
	{ 0x40235c98, "_raw_write_unlock" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x848b9552, "cdev_init" },
	{ 0x3febc5d1, "cdev_add" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");

