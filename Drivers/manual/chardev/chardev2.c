/*
 * chardev2.c - make symbol input/output device
 */

#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/poll.h>

#include "my_ioctl_codes.h"
#define SUCCESS 0
#define DEVICE_NAME "char_dev"
#define BUF_LEN 80

enum {
	CDEV_NOT_USED = 0,
	CDEV_EXCLUSIVE_OPEN = 1,
};

/* Is device open now? Serves to prevent concurrent access to
 * single device.
 */
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

/* Message that device will give out in case of request. */
static char message[BUF_LEN];

static struct class *cls;

static const unsigned int count_of_cdev = 2;

#define COUNT_OF_CDEV 2

static struct cdev array_cdev[COUNT_OF_CDEV];

/* Called when proccess tries to open device file. */
static int device_open(struct inode *inode, struct file *file)
{
	pr_info("device_open(%p)\n", file);
	
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	pr_info("device_release(%p, %p) in %s\n",
		       	inode, file, __func__);
	module_put(THIS_MODULE);
	return SUCCESS;
}

/* This function called when already opened proccess
 * tries to read file.
 */
static ssize_t device_read(struct file *file, 
				char __user *buffer,
				size_t length,
				loff_t *offset)
{
	/* Count of readed bytes. */
	int bytes_read = 0;
	/* How far has the proccess gone?
	 * Useful when message size more than buffer size.
	 */
	const char *message_ptr = message;
	/* We at the end of the message. */

	if (!*(message_ptr + *offset)) {
		*offset = 0; /* Reset offset. */
		return 0; /* Marker end of file. */
	}

	message_ptr += *offset;

	/* Actually put the data in the buffer. */
	while (length && *message_ptr) {
		/* *(buffer++) = *(message_ptr++) */
		/* Move data between kernel space and user space. */
		put_user(*(message_ptr++), buffer++);
		length--;
		bytes_read++;
	}

	pr_info("Read %d bytes, %ld left\n", bytes_read, length);

	*offset += bytes_read;

	return bytes_read;
}

/* Called when someont tries write into device file. */
static ssize_t device_write(struct file *file,
	       	const char __user *buffer,
		size_t length, loff_t *offset)
{
	int i;

	pr_info("%s(%p, %p, %ld)", __func__, file, buffer, length);

	for (i = 0; i < length && i < BUF_LEN; i++)
		get_user(message[i], buffer + i);
	return i;
}

/* This function called when proccess tries to call ioctl for 
 * device file */
static long
device_ioctl(struct file *file, unsigned int ioctl_num,
		unsigned long ioctl_param)
{
	int i;
	long ret = SUCCESS;

	/* We do not want deal with two processes simultaneously */
	if (atomic_cmpxchg(&already_open,
				CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN))
	return -EBUSY;

	switch (ioctl_num) {
	case IOCTL_SET_MSG: {
		
		char __user *tmp = (char __user *)ioctl_param;
		char ch;

		/* determine length of message */
		get_user(ch, tmp);
		for (i = 0; ch && i < BUF_LEN; i++, tmp++)
			get_user(ch, tmp);
		device_write(file, (char __user *)ioctl_param,
			       	i, NULL);
		break;
	}
	case IOCTL_GET_MSG: {
		loff_t offset = 0;
		/* Pass current message to called proccess.
		 * Derived ioctl_param is pointer that we fill.
		 */
		i = device_read(file, (char __user *)ioctl_param,
			       	99, &offset);
		/* Put zero at the end of the buffer.*/
		put_user('\0', (char __user *)ioctl_param + i);
		break;
	}
	case IOCTL_GET_NTH_BYTE: {
		/* This ioctl as an input as an output command */
		ret = (long)message[ioctl_param];
		break;
	}
	}
	/* Now we can accept following call. */
	atomic_set(&already_open, CDEV_NOT_USED);

	return ret;
}

/* Declaration of modules. */
static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release,
};
static dev_t dev;
/* Initialization of module and registration of character device. */
static int __init chardev2_init(void)
{
	/* Try to registrate character device */
	int ret_val = alloc_chrdev_region(&dev, 0,
		       	count_of_cdev, DRIVER_NAME);
	if (ret_val)
		goto error_alloc;
	int major_number = MAJOR(dev);
	for (int i = 0; i < COUNT_OF_CDEV; i++) {
		cdev_init(&array_cdev[i], &fops);
		/* Need rollback if error occurs */
		int ret = cdev_add(&array_cdev[i], dev + i, 1);
	}

	cls = class_create(DEVICE_FILE_NAME);
	for (int i = 0; i < COUNT_OF_CDEV; i++) {
		device_create(cls, NULL, dev + i, NULL,
			       	"device_%d", i);
	}
	pr_info("%s: %d Devices are created\n",
		       	__func__, COUNT_OF_CDEV);
	return 0;


error_alloc:
	unregister_chrdev_region(dev, COUNT_OF_CDEV);
	return -1;
}


static void __exit chardev2_exit(void) 
{
	for (int i = 0; i < COUNT_OF_CDEV; i++) {
		device_destroy(cls, dev + i);
	}
	class_destroy(cls);

	unregister_chrdev_region(dev, COUNT_OF_CDEV);
}

module_init(chardev2_init);
module_exit(chardev2_exit);

MODULE_LICENSE("GPL");
