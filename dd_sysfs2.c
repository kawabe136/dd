#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

// /sys/kernel/kobject_example
// bar  baz  foo

static struct kobject *example_kobj;

static int foo;
static int baz;
static int bar;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
       return sprintf(buf, "%s:%d\n", attr->attr.name, foo);
}
static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
       sscanf(buf, "%du", &foo);
       return count;
}

static ssize_t baz_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
       return sprintf(buf, "%s:%d\n", attr->attr.name, baz);
}
static ssize_t baz_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
       sscanf(buf, "%du", &baz);
       return count;
}

static ssize_t bar_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
       return sprintf(buf, "%s:%d\n", attr->attr.name, bar);
}
static ssize_t bar_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
       sscanf(buf, "%du", &bar);
       return count;
}

static struct kobj_attribute foo_attribute = __ATTR(foo, 0666, foo_show, foo_store);
static struct kobj_attribute baz_attribute = __ATTR(baz, 0666, baz_show, baz_store);
static struct kobj_attribute bar_attribute = __ATTR(bar, 0666, bar_show, bar_store);

static struct attribute *attrs[] = {
       &foo_attribute.attr,
       &baz_attribute.attr,
       &bar_attribute.attr,
       NULL,   /* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
       .attrs = attrs,
};

static int example_init(void)
{
       int retval;

       example_kobj = kobject_create_and_add("kobject_example", kernel_kobj);
       if (!example_kobj)
               return -ENOMEM;

       retval = sysfs_create_group(example_kobj, &attr_group);
       if (retval)
               kobject_put(example_kobj);

       return retval;
}

static void example_exit(void)
{
       kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Greg Kroah-Hartman <greg@kroah.com>");
