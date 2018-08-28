#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/irq.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/jiffies.h>


// sysfs file is created in the /sys/devices/virtual/dd_sensors/dd
// drwxr-xr-x 2 root root    0  7月 21 08:02 power/
// lrwxrwxrwx 1 root root    0  7月 21 08:02 subsystem -> ../../../../class/dd_sensors/
// -rw-r--r-- 1 root root 4096  7月 21 08:02 test
// -rw-r--r-- 1 root root 4096  7月 21 08:02 uevent

// /include/uapi/linux/time.h
//struct timeval {
//    __kernel_time_t     tv_sec;     /* seconds */
//    __kernel_suseconds_t    tv_usec;    /* microseconds */
//};

// since january 1, 1970(UTC)


int sysfs_setup(void);

struct dd_data{
  struct class*         test_class;
  struct device*        test_dev;
  struct timeval        tv;
};

static struct dd_data* g_data_po;

//sysfs
static ssize_t test_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct dd_data *data_po = g_data_po;
    unsigned long time;

	printk(KERN_INFO "sysfs show \n");
    
    do_gettimeofday(&data_po->tv);
    time = (data_po->tv.tv_sec)*1000000 + data_po->tv.tv_usec;

	printk(KERN_INFO "time sec:%lu\n", data_po->tv.tv_sec);
	printk(KERN_INFO "time usec:%lu\n", data_po->tv.tv_usec);
	printk(KERN_INFO "time usec:%lu\n", time);

  	return 0;
}

static ssize_t test_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
//	struct dd_data *data_po = g_data_po;
	int ret ;
	unsigned long n = 0;

   	if(!buf){
		return -EINVAL;
	}

	ret = strict_strtoul(buf, 10, &n);
	if(ret < 0){
		printk(KERN_INFO "invalid value\n");
 	}
	
	printk(KERN_INFO "n = %lu \n", n);

	return count;
}


static DEVICE_ATTR(test, 0644, test_show, test_store);

static struct attribute *dd_attributes[] =
{
  &dev_attr_test.attr,
  NULL
};

static struct attribute_group dd_attribute_group =
{
  .attrs = dd_attributes
};


int sysfs_setup()
{
    struct dd_data *data_po;
	int ret = 0; 

    data_po = kzalloc(sizeof(struct dd_data), GFP_KERNEL);
    if (!data_po){
        return -ENOMEM;
    }

    g_data_po =data_po;

	data_po->test_class =class_create(THIS_MODULE, "dd_sensors");
	if(IS_ERR(data_po->test_class)){
	  ret = PTR_ERR(data_po->test_class);
	  data_po->test_class = NULL;
	//  goto err_create_class;
        	}
 
    // device_create - creates a device and registers it with sysfs
	data_po->test_dev = device_create(data_po->test_class, NULL, 0, "%s", "dd");
	if(unlikely(IS_ERR(data_po->test_dev))){
	  ret = PTR_ERR(data_po->test_dev);
	  data_po->test_dev = NULL;
	 // goto err_create_test_device;
 	}

    // device_create_file - create sysfs attribute file for device.
//	ret =device_create_file(data_po->test_dev, &dev_attr_test);
    ret = sysfs_create_group( &data_po->test_dev->kobj, &dd_attribute_group) ;
	if(ret){
	//  goto err_create_test_device_file;
	}

//err_create_singlethread_workqueue:

	return 0;
}

static int dd_init(void)
{
    printk(KERN_INFO "****************************** \n");
    printk(KERN_INFO "   sysfs test driver loaded\n");
    printk(KERN_INFO "****************************** \n");

	sysfs_setup();

	return 0;
}

static void dd_exit(void)
{
	struct dd_data *data_po = g_data_po;

    printk(KERN_INFO "******************************** \n");
    printk(KERN_INFO "   sysfs test driver unloaded\n");
    printk(KERN_INFO "******************************** \n");

//	device_remove_file(data_po->test_dev);
    sysfs_remove_group( &data_po->test_dev->kobj, &dd_attribute_group) ;
	device_unregister(data_po->test_dev); //do work
   	class_unregister(data_po->test_class);
    kfree(data_po);
//     	class_destroy(data_po->test_class);
//	device_destroy(data_po->test_class);
	printk(KERN_INFO "driver unloaded\n");
}


module_init(dd_init); 
module_exit(dd_exit);

MODULE_AUTHOR( "Isamu Kawabe <ikwb136@icloud.com>" ) ;
MODULE_DESCRIPTION( "TEST" ) ;
MODULE_LICENSE( "GPL" ) ;

