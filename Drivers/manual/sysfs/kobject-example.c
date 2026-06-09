/*
 * i rewrite this file to better understand this conceptions
 */

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

static int foo;
static int baz;
static int bar;

static ssize_t foo_show(struct kobject *kobj,
	       		struct kobj_attribute *attr,
			char *buf)
{
	return sysfs_emit(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj,
			 struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	int ret;

	ret = kstrtoint(buf, 10, &foo);
	if (ret < 0)
		return ret;
	return count;
}

/* not world-writable */
static struct kobj_attribute foo_attribute =
	__ATTR(foo, 0664, foo_show, foo_store);
/*
 * show and store but we looking at the attribute 
 */
static ssize_t b_show(struct kobject *kobj,
			struct kobj_attribute *attr,
			char *buf)
{
	int var;

	if (strcmp(attr->attr.name, "baz") == 0)
		var = baz;
	else
		var = bar;
	return sysfs_emit(buf, "%d\n", var);
}

static ssize_t b_store(struct kobject *kobj,
	       		struct kobj_attribute *attr,
			const char *buf, size_t count)
{
	int var, ret;

	ret = kstrtoint(buf, 10, &var);
	if (ret < 0)
		return ret;
	if (strcmp(attr->attr.name, "baz") == 0)
		baz = var;
	else 
		bar = var;
	return count;
}

static struct kobj_attribute baz_attribute =
	__ATTR(baz, 0664, b_show, b_store);
static struct kobj_attribute bar_attribute = 
	__ATTR(bar, 0644, b_show, b_store);

/*
 * Create a group of attributes so that we can create
 *  and destroy them all at once
 */
static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&baz_attribute.attr,
	&bar_attribute.attr,
	NULL, /* need to NULL terminate the list of attributes */
};

static const struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;

static int __init example_init(void)
{
	int retval;
	/* Create a simple kobject 
	 * with the name of "kobject_example",
	 * located under /sys/kernel
	 */
	example_kobj = kobject_create_and_add("kobject_example_dir",
			NULL);
	if (!example_kobj)
		return -ENOMEM;
	/* Create the files associated with this kobject */
	retval = sysfs_create_group(example_kobj, &attr_group);
	if (retval)
		kobject_put(example_kobj);
	return retval;
}

static void __exit example_exit(void) 
{
	kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
