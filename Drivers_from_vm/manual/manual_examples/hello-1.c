/*
 * hello-1.c - simplest kernel module
 */
#include <linux/kernel.h> /* necessary for pr_info() */
#include <linux/module.h> /* necessary for all modules */

int init_module(void)
{
	pr_info("Hello world 1.\n");

	return 0;
}

 void cleanup_module(void)
{ 
	pr_info("Goodbye world 1.\n");
}
MODULE_LICENSE("GPL");
