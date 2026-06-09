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
	{ 0x93d6dd8c, "complete_all" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x608741b5, "__init_swait_queue_head" },
	{ 0x443ad987, "kthread_create_on_node" },
	{ 0xbc390422, "kthread_stop" },
	{ 0xa2c0b7ff, "wake_up_process" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x25974000, "wait_for_completion" },
	{ 0x92997ed8, "_printk" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");

