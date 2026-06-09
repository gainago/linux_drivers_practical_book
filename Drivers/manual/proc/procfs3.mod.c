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
	{ 0xd9ecfe5d, "proc_set_size" },
	{ 0x2e6780ca, "proc_set_user" },
	{ 0x92997ed8, "_printk" },
	{ 0x56c60877, "module_put" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0x40c97d4d, "try_module_get" },
	{ 0x666e61e9, "remove_proc_entry" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x2cf56265, "__dynamic_pr_debug" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xf45c3f31, "proc_create" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");

