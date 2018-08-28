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

int psals_setup(void);

struct test_info{
  struct class*         test_class;
  struct device*            ls_dev;
};

static struct test_info* lp_info;

//sysfs
static ssize_t ls_adc_show(struct device *dev, struct device_attribute *attr, char *buf)
{
//	struct test_info *lpi = lp_info;

	printk(KERN_ALERT "sysfs show \n");

  	return 0;
}

static ssize_t ls_adc_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
//	struct test_info *lpi = lp_info;
	int ret ;
	unsigned long ps_en = 0;

   	if(!buf){
		return -EINVAL;
	}

	ret = strict_strtoul(buf, 10, &ps_en);
	if(ret < 0){
		printk(KERN_ALERT "invalid value\n");
 	}
	
	printk(KERN_ALERT "ps_en = %lu \n", ps_en);

	return count;
}

static DEVICE_ATTR(ls_adc, 0644, ls_adc_show, ls_adc_store);

int psals_setup()
{
    struct test_info *lpi;
	int ret = 0; 

    lpi = kzalloc(sizeof(struct test_info), GFP_KERNEL);
    if (!lpi){
        return -ENOMEM;
    }

    lp_info =lpi;

	lpi->test_class =class_create(THIS_MODULE, "optical_sensors");
	if(IS_ERR(lpi->test_class)){
	  ret = PTR_ERR(lpi->test_class);
	  lpi->test_class = NULL;
	//  goto err_create_class;
        	}

	lpi->ls_dev = device_create(lpi->test_class, NULL, 0, "%s", "lightsensor");
	if(unlikely(IS_ERR(lpi->ls_dev))){
	  ret = PTR_ERR(lpi->ls_dev);
	  lpi->ls_dev = NULL;
	 // goto err_create_ls_device;
 	}

	ret =device_create_file(lpi->ls_dev, &dev_attr_ls_adc);
	if(ret){
	//  goto err_create_ls_device_file;
	}

//err_create_singlethread_workqueue:

	return 0;
}

static int psals_init(void)
{
    printk(KERN_INFO "****************************** \n");
    printk(KERN_INFO "   sysfs test driver loaded\n");
    printk(KERN_INFO "****************************** \n");

	psals_setup();

	return 0;
}

static void psals_exit(void)
{
	struct test_info *lpi = lp_info;

    printk(KERN_INFO "******************************** \n");
    printk(KERN_INFO "   sysfs test driver unloaded\n");
    printk(KERN_INFO "******************************** \n");

//	device_remove_file(lpi->ls_dev);
	device_unregister(lpi->ls_dev); //do work
   	class_unregister(lpi->test_class);
//     	class_destroy(lpi->test_class);
//	device_destroy(lpi->test_class);
	printk(KERN_ALERT "derver unloaded\n");
}


module_init(psals_init); 
module_exit(psals_exit);

MODULE_LICENSE("Dual BSD/GPL");

