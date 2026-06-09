/*
 * sleep.c - make file /proc and if many proccesses would
 * try to open this file than all be waiting
 */

#include <linux/kernel.h> /* for work with kernel */
#include <linux/module.h> /* for modules */
#include <linux/proc_fs.h> /* for using procfs */
#include <linux/sched.h> /* for uthanazing proccesses and wake up */
#include <linux/uaccess.h> /* for get_user and put_user */
#include <linux/version.h>
#include <linux/signal.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

/* Here we contain last get message, proving possibility
 * of processing a input
 */
#define MESSAGE_LENGTH 80
static char message[MESSAGE_LENGTH];

static struct proc_dir_entry *our_proc_file;
#define PROC_ENTRY_FILENAME "sleep"

static ssize_t module_output(struct file *file,
				char __user *buf,
				size_t len,
				loff_t *offset)
{
	static int finished = 0;
	int i = 0;
	char output_msg[MESSAGE_LENGTH + 30];

	/* Return 0, marked end of file. */
	if (finished) {
		finished = 0;
		return 0;
	}

	sprintf(output_msg, "Last input:%s\n", message);
	for (i = 0; i < len && output_msg[i]; i++)
		put_user(output_msg[i], buf + i);
	finished = 1;
	return i; /* return count of readed bytes */

}

/* this function gets input from user, when he writes into
 * /proc file
 */
static ssize_t module_input(struct file *file,
				const char __user *buf,
				size_t length,
				loff_t *offset)
{
	int i = 0;
	for (i = 0; i < MESSAGE_LENGTH - 1 && i < length; i++)
		get_user(message[i], buf + i);
	/* we need null terminated string */
	message[i] = '\0';

	return i;
}

/* 1, if the file has already opend */
static atomic_t already_open = ATOMIC_INIT(0);

/* Processes queue, waiting access to file */
static DECLARE_WAIT_QUEUE_HEAD(waitq);

/* called when opening file /proc. */
static int module_open(struct inode *inode, struct file *file)
{
	/* if flag O_NONBLOCK set, whan process do not want to wait
	 * if the file is opened. In than case return -EAGAIN.
	 */
	if ((file->f_flags & O_NONBLOCK) &&
		       	atomic_read(&already_open)) {
		return -EAGAIN;
	}

	try_module_get(THIS_MODULE);

	while (atomic_cmpxchg(&already_open, 0, 1)) {
		int i, is_sig = 0;
		/* This function send the current process
		 * and all its system call to sleep. Executing will
		 * continue after wake_up(&waitq)( into module_close
		 * or after sending Ctrl+C signal to this process
		 */
		wait_event_interruptible(waitq,
			       	!atomic_read(&already_open));
		if (signal_pending(current)) {
			module_put(THIS_MODULE);
			return -EINTR;
		}
	}
	return 0;
}

/* called with closing file /proc. */
static int module_close(struct inode *inode, struct file *file)
{
	/* Set already_open to zero to one of waiting processes
	 * got resourse.
	 */
	atomic_set(&already_open, 0);

	/* Waking up all processes into waitq. */
	wake_up(&waitq);

	module_put(THIS_MODULE);

	return 0;
}

/* Structures for registration as /proc file
 * with all linked functions
 */
#ifdef HAVE_PROC_OPS 
static const struct proc_ops file_ops_4_our_proc_file = {
	.proc_read = module_output, /* Reading from file */
	.proc_write = module_input, /* Writing to file */
	.proc_open = module_open, /* Called to opening /proc file */
	.proc_release = module_close, /* Called to close this file */
};
#else 
static const struct file_operations file_ops_4_our_proc_file = {
	.read = module_output,
	.write = module_input,
	.open = module_open,
	.release = module_close,
};
#endif

static int __init sleep_init(void)
{
	our_proc_file = 
		proc_create(PROC_ENTRY_FILENAME, 0666, NULL,
				&file_ops_4_our_proc_file);
	if (our_proc_file == NULL) {
		pr_debug("Error: Could not initialize /proc/%s",
				PROC_ENTRY_FILENAME);
		return -ENOMEM;
	}

	pr_info("/proc/%s created\n", PROC_ENTRY_FILENAME);

	return 0;
}

/* Clear - unset registration file from /proc. This may be dangerous
 * if waitq contain waiting processes, because they
 * sleeping in open() function and it will be unload
 */
static void __exit sleep_exit(void)
{
	proc_remove(our_proc_file);
	pr_debug("/proc/%s removed\n", PROC_ENTRY_FILENAME);
}

module_init(sleep_init);
module_exit(sleep_exit);

MODULE_LICENSE("GPL");
