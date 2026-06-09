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
	{ 0xb5b54b34, "_raw_spin_unlock" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x3854774b, "kstrtoll" },
	{ 0x208080f5, "_dev_info" },
	{ 0x8f5fd28a, "input_event" },
	{ 0x5d706d5c, "_dev_err" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xc019be07, "input_register_device" },
	{ 0x7fae6f95, "vinput_unregister" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xc8cadbb4, "vinput_register" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "vinput");

