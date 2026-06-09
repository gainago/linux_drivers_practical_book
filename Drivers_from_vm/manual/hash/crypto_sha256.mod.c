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
	{ 0x903df714, "crypto_alloc_shash" },
	{ 0x52c5c991, "__kmalloc_noprof" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0xbfe9b585, "crypto_shash_update" },
	{ 0x450e55e, "crypto_shash_final" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x37a0cba, "kfree" },
	{ 0x8dfc081a, "crypto_destroy_tfm" },
	{ 0x92997ed8, "_printk" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");

