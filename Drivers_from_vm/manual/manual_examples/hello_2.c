/*
 * hello-2.c - demonstdrates of macros module_init() and
 * module_exit()
 */
#include <linux/init.h> /* for macroses */
#include <linux/kernel.h> /* for pr_info() */
#include <linux/module.h> /* requirement for all modules */ 

static int hello_2_init(void)
{
	pr_info("Hello, world 2\n");
	pr_info("After breakpoint\n");
	return 0;
}

static void  hello_2_exit(void)
{
	pr_info("Goodbye, world 2\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);

MODULE_LICENSE("GPL");

		
