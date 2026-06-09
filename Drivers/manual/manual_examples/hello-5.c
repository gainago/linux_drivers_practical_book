/*
 * hello-5.c - demonstrates passing arguments command line to module
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");

static short int my_short = 1;
static int my_int = 42000;
static long int my_long = 999999;
static char *my_string = "string \"my_string\"";
static int my_int_array[2] = { 420, 420 };
static int arr_argc = 0;

module_param(my_short, short, S_IRUGO);
MODULE_PARM_DESC(my_short, "A short integer");

module_param(my_int, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(my_int, "An integer");

module_param(my_long, long, S_IRUGO);
MODULE_PARM_DESC(my_long, "A long integer");

module_param(my_string, charp, 0);
MODULE_PARM_DESC(my_string, "A character string");

module_param_array(my_int_array, int, &arr_argc, S_IRUGO);
MODULE_PARM_DESC(my_int_array, "An array of integers");

static int __init hello_5_init(void)
{
	int i = 0;

	pr_info("Hello, world 5\n===================\n");
	pr_info("my_short is a short integer: %hd\n", my_short);
	pr_info("my_int is an integer: %d\n", my_int);
	pr_info("my_long is a long integer: %ld\n", my_long);
	pr_info("my_string is a string: %s\n", my_string);

	for (i = 0; i < ARRAY_SIZE(my_int_array); i++) {
		pr_info("my_int_array[%d] = %d\n",
			       	i, my_int_array[i]);
	}

	pr_info("got %d argument for my_int_array.\n", arr_argc);
	return 0;
}

static void __exit hello_5_exit(void)
{
	pr_info("Goodbye, world 5\n");
}

module_init(hello_5_init);
module_exit(hello_5_exit);
