/*
 * hello-3.c - demonstrative mactoces __init, __initdata and exit
 */
#include <linux/init.h> /* necessary for macros */
#include <linux/kernel.h> /* necessary for pr_info() */
#include <linux/module.h>

static int hello3_data __initdata = 3;

static int __init hello_3_init(void)
{
	pr_info("Hello, world %d\n", hello3_data);
	return 0;
}

static void __exit hello_3_exit(void) 
{
	pr_info("Goodbye, world 3\n");
}

module_init(hello_3_init);
module_exit(hello_3_exit);

MODULE_LICENSE("GPL");
