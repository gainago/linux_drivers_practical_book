/*
 * print_string.c - sending output to tty, which we working on
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h> /* for current */
#include <linux/tty.h> /* for declaration tty */

static void print_string(char *str)
{
	/* tty for current task. */
	struct tty_struct *my_tty = get_current_tty();

	/* If my_tty == NULL, than current task has no tty, and we can not 
	 * make output.
	 */
	if (my_tty) {
		const struct tty_operations *tty_ops = my_tty->driver->ops;
		(tty_ops->write)(my_tty, /* tty ourself,
					    nessesary to polymorphism
					  */
				str, strlen(str));
		(tty_ops->write)(my_tty, "\r\n", 2);
	}
}

static int __init print_string_init(void)
{
	print_string("The module has been inserted. Hello world!");
	return 0;
}

static void __exit print_string_exit(void)
{
	print_string("The module has been removed. Farewell world!");
}

module_init(print_string_init);
module_exit(print_string_exit);

MODULE_LICENSE("GPL");



