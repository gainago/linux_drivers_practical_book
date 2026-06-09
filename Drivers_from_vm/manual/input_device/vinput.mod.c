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

KSYMTAB_FUNC(vinput_register, "", "");
KSYMTAB_FUNC(vinput_unregister, "", "");

SYMBOL_CRC(vinput_register, 0xc8cadbb4, "");
SYMBOL_CRC(vinput_unregister, 0x7fae6f95, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x848b9552, "cdev_init" },
	{ 0x3febc5d1, "cdev_add" },
	{ 0x124d226c, "class_register" },
	{ 0x37e16a57, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x3854774b, "kstrtoll" },
	{ 0xf39d099a, "input_unregister_device" },
	{ 0x2a885f5b, "device_unregister" },
	{ 0x658ca942, "_dev_warn" },
	{ 0x9e4f6a2f, "class_unregister" },
	{ 0x7696f8c7, "__list_add_valid_or_report" },
	{ 0x950eb34e, "__list_del_entry_valid_or_report" },
	{ 0x56c60877, "module_put" },
	{ 0x37a0cba, "kfree" },
	{ 0x2cf56265, "__dynamic_pr_debug" },
	{ 0xa916b694, "strnlen" },
	{ 0x5a921311, "strncmp" },
	{ 0xc4aa18f, "kmalloc_caches" },
	{ 0xd8ba0fc0, "__kmalloc_cache_noprof" },
	{ 0x40c97d4d, "try_module_get" },
	{ 0x9536f334, "input_allocate_device" },
	{ 0x2980192c, "dev_set_name" },
	{ 0x6dc0f498, "device_register" },
	{ 0x75ca79b5, "__fortify_panic" },
	{ 0x208080f5, "_dev_info" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xb5b54b34, "_raw_spin_unlock" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x68a12ab8, "rep_movs_alternative" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");

