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
	{ 0xe2964344, "__wake_up" },
	{ 0x56c60877, "module_put" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xc3aaf0a9, "__put_user_1" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x6dbed688, "proc_remove" },
	{ 0x40c97d4d, "try_module_get" },
	{ 0x33b0e10c, "const_pcpu_hot" },
	{ 0xe2c17b5d, "__SCT__might_resched" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x1000e51, "schedule" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x92540fbf, "finish_wait" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x167e7f9d, "__get_user_1" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xf45c3f31, "proc_create" },
	{ 0x2cf56265, "__dynamic_pr_debug" },
	{ 0x92997ed8, "_printk" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");

