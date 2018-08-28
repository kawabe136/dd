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


int sysfs_setup(void);

struct dd_data{
  struct class*         test_class;
  struct device*        test_dev;
};

static struct dd_data* lp_info;

//sysfs
static ssize_t test_show(struct device *dev, struct device_attribute *attr, char *buf)
{
//	struct dd_data *infop = lp_info;

	printk(KERN_ALERT "sysfs show \n");

  	return 0;
}

static ssize_t test_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
//	struct dd_data *infop = lp_info;
	int ret ;
	unsigned long n = 0;

   	if(!buf){
		return -EINVAL;
	}

	ret = strict_strtoul(buf, 10, &n);
	if(ret < 0){
		printk(KERN_ALERT "invalid value\n");
 	}
	
	printk(KERN_ALERT "n = %lu \n", n);

	return count;
}

static DEVICE_ATTR(test, 0644, test_show, test_store);

int sysfs_setup()
{
    struct dd_data *infop;
	int ret = 0; 

    infop = kzalloc(sizeof(struct dd_data), GFP_KERNEL);
    if (!infop){
        return -ENOMEM;
    }

    lp_info =infop;

	infop->test_class =class_create(THIS_MODULE, "dd_sensors");
	if(IS_ERR(infop->test_class)){
	  ret = PTR_ERR(infop->test_class);
	  infop->test_class = NULL;
	//  goto err_create_class;
        	}
 
    // device_create - creates a device and registers it with sysfs
	infop->test_dev = device_create(infop->test_class, NULL, 0, "%s", "dd");
	if(unlikely(IS_ERR(infop->test_dev))){
	  ret = PTR_ERR(infop->test_dev);
	  infop->test_dev = NULL;
	 // goto err_create_test_device;
 	}

    // device_create_file - create sysfs attribute file for device.
	ret =device_create_file(infop->test_dev, &dev_attr_test);
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
	struct dd_data *infop = lp_info;

    printk(KERN_INFO "******************************** \n");
    printk(KERN_INFO "   sysfs test driver unloaded\n");
    printk(KERN_INFO "******************************** \n");

//	device_remove_file(infop->test_dev);
	device_unregister(infop->test_dev); //do work
   	class_unregister(infop->test_class);
    kfree(infop);
//     	class_destroy(infop->test_class);
//	device_destroy(infop->test_class);
	printk(KERN_ALERT "driver unloaded\n");
}


module_init(dd_init); 
module_exit(dd_exit);

MODULE_AUTHOR( "Isamu Kawabe <ikwb136@icloud.com>" ) ;
MODULE_DESCRIPTION( "TEST" ) ;
MODULE_LICENSE( "GPL" ) ;

