/*
 * hello-sysfs.c - example of using sysfs
 */
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>

static struct kobject *my_module;

/* A variable that we will change. */
static int my_variable = 0;

static ssize_t my_variable_show(struct kobject *kobj,
				struct kobj_attribute *attr,
			        char *buf)
{
	return sprintf(buf, "%d\n", my_variable);
}

static ssize_t my_variable_store(struct kobject *kobj,
				 struct kobj_attribute *attr,
				 char *buf, size_t count)
{
	 int ret = sscanf(buf, "%d", &my_variable);
	 return ret;
	
}

static struct kobj_attribute my_variable_attribute =
	__ATTR(my_variable, 0660, my_variable_show, (void *)
			my_variable_store);
static int  my_module_init(void)
{
	int error = 0;

	pr_info("my module: initialization\n");

	my_module = kobject_create_and_add("my module",
		       			kernel_kobj);
	if (!my_module)
		return -ENOMEM;

	error = sysfs_create_file(my_module,
		       	&my_variable_attribute.attr);
	if (!error) {
		pr_info("successful to create the myvariable"
				"in /sys/kernel/my module\n");
	}

	return error;
}

static void  my_module_exit(void)
{
	pr_info("my module: Exit success\n");
	kobject_put(my_module);
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");

