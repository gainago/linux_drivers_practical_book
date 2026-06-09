/*
 * procfs4.c - make "file" in /proc
 * this program used for manage file /proc libraly seq_file
 */

#include <linux/kernel.h> /* for work with kernel */
#include <linux/module.h> /* for modules */
#include <linux/proc_fs.h> /* for use procfs */
#include <linux/seq_file.h> /* for seq_file */
#include <linux/version.h> /* for KERNEL_VERSION */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROC_NAME "iter"
/* this function called in start of sequence
 * when first time read file into /proc
 * after reading is ended (last in action sequence)
 */
static void *my_seq_start(struct seq_file *s, loff_t *pos)
{
	static unsigned long counter = 0;
	/* start a new sequence */
	if (*pos == 0) {
		/* yes? => return not null value
		 * for start sequence 
		 */
		return &counter;
	}

	/* No? => this is end of sequence,
	 *  return NULL for end of reading 
	 */
	*pos = 0;
	return NULL;
}

/* this function called after start of sequence of operations
 * this call repeat untill it returns NULL (after this sequence end)
 */
static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	unsigned long *tmp_v = (unsigned long *)v;
	(*tmp_v)++;
	(*pos)++;
	return (void *)pos;
}

/*this function called in the end of sequence */
static void my_seq_stop(struct seq_file *s, void *v)
{
	/* may do not nothing, we use in start() static values */
}

/* this function called for every "step" of sequence */
static int my_seq_show(struct seq_file *s, void *v)
{
	loff_t *spos = (loff_t *)v;
	seq_printf(s, "%Ld\n", *spos);
       return 0;
}

/* this structure forms "function" for manage sequence */
static struct seq_operations my_seq_ops = {
	.start = my_seq_start,
	.next = my_seq_next,
	.stop = my_seq_stop,
	.show = my_seq_show,
};

static int my_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &my_seq_ops);
};

/* this structure forms "function", that manages /proc file */
#ifdef HAVE_PROC_OPS
static const struct proc_ops my_file_ops = {
	.proc_open = my_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
};
#else
static const struct file_operations my_file_ops = {
	.open = my_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};
#endif

static int __init procfs4_init(void)
{
	struct proc_dir_entry *entry;

	entry = proc_create(PROC_NAME, 0, NULL, &my_file_ops);
	if (entry == NULL) {
		remove_proc_entry(PROC_NAME, NULL);
		pr_info("Error: Could not initialize /proc/%s\n",
			       	PROC_NAME);
		return -ENOMEM;
	}
	return 0;
}

static void __exit procfs4_exit(void)
{
	remove_proc_entry(PROC_NAME, NULL);
	pr_info("/proc/%s removed\n", PROC_NAME);
}

module_init(procfs4_init);
module_exit(procfs4_exit);

MODULE_LICENSE("GPL");
