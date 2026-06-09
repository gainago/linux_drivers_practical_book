#include <linux/module.h>
#include <linux/fs.h> // file operations
#include <linux/uaccess.h> //user space -> kernel 
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cdev.h> // definitions to character devices 
#include <linux/device.h>
#include <linux/err.h> // IS_ERR_VALUE()

static char *kbuf = NULL;
static dev_t first;
static unsigned int count = 1; // count of devices
static int my_minor = 0;
static struct cdev *my_cdev = NULL;
static struct class *my_class = NULL;
static struct device *my_device = NULL;

#define MYDEV_NAME "my_character_device"
#define KBUF_SIZE (size_t)((10) * PAGE_SIZE)


//file inode and a file desctiptor from openning process
static int char_dev_open(struct inode* inode, struct file* file_ptr)
{
	static int counter = 0;
	char* kbuf = (char*)kmalloc(KBUF_SIZE, GFP_KERNEL | __GFP_ZERO);
	if(!kbuf) {
		printk(KERN_CRIT "Error kmalloc\n");
		goto err_kmalloc;
	}
	file_ptr->private_data = kbuf;

	printk(KERN_INFO "Opening device %s:\n\n", MYDEV_NAME);

	printk(KERN_INFO "counter = %d\n", counter);
	printk(KERN_INFO "module refcounter = %d\n", module_refcount(THIS_MODULE));

	return 0;
err_kmalloc:
	return -1;
}

static int char_dev_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Releasing device %s:\n\n", MYDEV_NAME);
	char* kbuf = file->private_data;
	if(kbuf) {
		kfree(kbuf);
		kbuf = file->private_data = NULL;
	}

	return 0;
}

static ssize_t char_dev_read(struct file *file, char __user *buf,
		size_t lbuf, loff_t *ppos)
{
	char* kbuf = file->private_data;

	size_t nbytes_to_copy = 0;
	if(*ppos + lbuf <= KBUF_SIZE) {
	       nbytes_to_copy = lbuf;
	}
	else {
		nbytes_to_copy = KBUF_SIZE - *ppos;
	}
	size_t copied_bytes = nbytes_to_copy
	       	-  copy_to_user(buf, kbuf + *ppos, nbytes_to_copy);
	*ppos += copied_bytes;

	printk(KERN_INFO "Read device %s: Readed bytes = %zu : *ppos = %d\n\n",
		       	MYDEV_NAME, copied_bytes, (int)*ppos);
	return copied_bytes;
}

static ssize_t char_dev_write(struct file *file,
	       	const char __user *buf, size_t lbuf, loff_t *ppos)
{
	char* kbuf = file->private_data;

	size_t nbytes_to_copy = 0;
	if(*ppos + lbuf <= KBUF_SIZE) {
	       nbytes_to_copy = lbuf;
	}
	else {
		nbytes_to_copy = KBUF_SIZE - *ppos;
	}
	size_t copied_bytes = lbuf - copy_from_user(kbuf + *ppos, buf, nbytes_to_copy);
	*ppos += copied_bytes;
		
	printk(KERN_INFO "Write device %s: nbytes = %zu: ppos = %d\n\n",
		       	MYDEV_NAME, copied_bytes, (int)*ppos );
	return copied_bytes;
}

static loff_t char_dev_lseek(struct file *file, loff_t offset, int flag) {
	loff_t wanted_position = -1;
	switch(flag)
	{
		case SEEK_SET: 
			wanted_position = offset;
			break;
		case SEEK_CUR: 
			wanted_position = file->f_pos + offset;
			break;
		case SEEK_END:
			wanted_position = KBUF_SIZE + offset;
			break;
		default:
			return -EINVAL; //invalin argument
	}
	loff_t position = wanted_position < KBUF_SIZE ?
	       	wanted_position : KBUF_SIZE - 1;
	position = position >= 0 ? position : 0;
	file->f_pos = position;

	printk( KERN_INFO "Seeking to %ld position\n", (long)position);
	
	return position;
}


static const struct file_operations char_dev_fops = {
	.owner = THIS_MODULE,
	.read = char_dev_read,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release,
	.llseek = char_dev_lseek
};

static int __init init_char_dev(void)
{
	kbuf = (char*)kmalloc(KBUF_SIZE, GFP_KERNEL | __GFP_ZERO);
	if(!kbuf) {
		printk(KERN_CRIT "Error kmalloc\n");
		goto err_kmalloc;
	}
	
	if(alloc_chrdev_region(&first, my_minor, count, MYDEV_NAME) < 0) {
		printk(KERN_CRIT "Error alloc_chrdev_region\n");
		goto err_alloc_chrdev_region;
	}
	printk(KERN_INFO "Major number = %d\n", MAJOR(first) );

	my_cdev = cdev_alloc();
	if(!my_cdev) {
		printk(KERN_CRIT "Error cdev_alloc()\n");
		goto err_cdev_alloc;
	}
	
	cdev_init(my_cdev, &char_dev_fops);
	cdev_add(my_cdev, first, count);

	my_class = class_create("my_char_dev");
	if(IS_ERR(my_class)) {
		printk("Error class_create(\"my_char_dev\")\n");
		goto err_class_create;
	}
	my_device = device_create(my_class, NULL, first, "%s", "my_char_device");
	if(IS_ERR(my_device)) {
		printk("Error device_create\n");
		goto err_device_create;
	}


	printk(KERN_INFO "Created device class :%s\n", "my_char_device");

	return 0;
err_device_create:
	class_destroy(my_class);
err_class_create:
	cdev_del(my_cdev);
err_cdev_alloc:
	unregister_chrdev_region(first, count);
err_alloc_chrdev_region:
	kfree(kbuf);
err_kmalloc:
	return -1;
}

static void __exit cleanup_char_dev(void)
{
	device_destroy(my_class, first);
	class_destroy(my_class);
	cdev_del(my_cdev);
	unregister_chrdev_region(first, count);
	kfree(kbuf); //clean also global buffer

	printk( KERN_INFO "Leaving\n");
}

module_init(init_char_dev);
module_exit(cleanup_char_dev);

MODULE_LICENSE("GPL");

