/*
 * example_mutex.c
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>

static DEFINE_MUTEX(my_mutex);

static int example_mutex_init(void)
{
	int ret = -1;

	pr_info("example_mutex init\n");

	ret = mutex_trylock(&my_mutex);
	if (ret != 0) {
		pr_info("mutex is locked\n");

		if (mutex_is_locked(&my_mutex) == 0)
			pr_info("The mutex failed to lock\n");
		mutex_unlock(&my_mutex);
		pr_info("mutex si unlocked\n");
	} else 
		pr_info("Failed to lock\n");

	return 0;
}

static void example_mutex_exit(void)
{
	pr_info("example_mutex exit\n");
}

module_init(example_mutex_init);
module_exit(example_mutex_exit);

MODULE_DESCRIPTION("Mutex example");

MODULE_LICENSE("GPL");

