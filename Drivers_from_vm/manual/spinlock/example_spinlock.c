/*
 * example_spinlock.c
 */
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>

static DEFINE_SPINLOCK(sl_static);
static spinlock_t sl_dynamic;

static void example_spinlock_static(void)
{
	unsigned long flags;

	spin_lock_irqsave(&sl_static, flags);
	pr_info("Locked static spinlock\n");
	/* Save doing task. Using 100 %CPU
	 * time consuming should be less than 100
	 * milleseconds
	 */
	spin_unlock_irqrestore(&sl_static, flags);
	pr_info("Unlocked static spinlock\n");
}

static void example_spinlock_dynamic(void)
{
	unsigned long flags;

	spin_lock_init(&sl_dynamic);
	spin_lock_irqsave(&sl_dynamic, flags);
	pr_info("Locked dynamic(in term on \
	       	initialization spinlock\n");

	spin_unlock_irqrestore(&sl_dynamic, flags);
	pr_info("Unlocked dynamic spinlock\n");
}

static int example_spinlock_init(void)
{
	pr_info("example spinlock started\n");
	example_spinlock_static();
	example_spinlock_dynamic();

	return 0;
}

static void example_spinlock_exit(void)
{
	pr_info("example spinlock exit\n");
}

module_init(example_spinlock_init);
module_exit(example_spinlock_exit);

MODULE_DESCRIPTION("Spinlock example");
MODULE_LICENSE("GPL");

