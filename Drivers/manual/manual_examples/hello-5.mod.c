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
	{ 0x361be725, "param_ops_int" },
	{ 0x7c8b1794, "param_ops_long" },
	{ 0xa5315153, "param_ops_charp" },
	{ 0xd8c22202, "param_array_ops" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x2254e1ff, "param_ops_short" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");

