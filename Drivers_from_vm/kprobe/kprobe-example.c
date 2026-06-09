/*
 * kprobe-example.c
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

static char symbol[KSYM_NAME_LEN] = "__x64_sys_write";
module_param_string(symbol, symbol, KSYM_NAME_LEN, 0644);

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name = symbol,
};

/* kprobe pre handler: called just berore
 * the probed instruction is executed */
static int __kprobes handler_pre(struct kprobe *p,
	       	struct pt_regs *regs)
{
#ifdef CONFIG_X86
		pr_info("<%s> p->addr = 0x%p,"
			"ip = %lx, flags = 0x%lx\n",
		       	p->symbol_name, p->addr,
		       	regs->ip, regs->flags);
#endif
	return 0;
}

static void __kprobes handler_post(struct kprobe *p,
	       	struct pt_regs *regs,
	       unsigned long flags)
{
#ifdef CONFIG_X86
		pr_info("<%s> p->addr = 0x%p,"
			"ip = %lx, flags = 0x%lx\n",
		       	p->symbol_name, p->addr,
		       	regs->ip, regs->flags);
#endif
}

static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe fained, returned %d\n",
			       	ret);
		return ret;
	}
	pr_info("Planted kprobe at %p\n", kp.addr);
	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init)
module_exit(kprobe_exit);;;;;
MODULE_DESCRIPTION("sample kernel module"
	       "showing the use of kprobes");
MODULE_LICENSE("GPL");
