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


struct gp030_info{
  struct class*		  	gp030_class;
  struct device*	 		ls_dev;
  int  			test;
  struct workqueu_struct* 		als_wq;
  struct delayed_work	  	als_work;
  uint16_t 			als_delay_time;
};

static struct gp030_info*   lp_info;
int i;



//sysfs
static ssize_t ls_adc_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct gp030_info *lpi = lp_info;

	printk(KERN_ALERT "driver loaded wan wan %d\n",lpi->test);

  	return 0;
}

static ssize_t ls_adc_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct gp030_info *lpi = lp_info;
	int ret, ps_en = -1;

   	if(!buf){
		return -EINVAL;
	}

	ret = strict_strtoul(buf, 10, &ps_en);
	if(ret < 0){
		printk(KERN_ALERT "invalid value\n");
 	}
	
	lpi->test = ps_en;

	printk(KERN_ALERT "driver loaded wan wan %d\n",lpi->test);

	return count;
}

static DEVICE_ATTR(ls_adc, 0644, ls_adc_show, ls_adc_store);

static int psals_setup()
{
	int ret = 0; 
 	struct gp030_info *lpi;

	lpi = kzalloc(sizeof(struct gp030_info), GFP_KERNEL);
 	if (!lpi){
		return -ENOMEM;
	}
        lpi->test = 2;
        lpi->als_delay_time = 200;
	lpi->als_wq = create_singlethread_workqueue("gp030_als_wq");

	lp_info = lpi;

	lpi->gp030_class =class_create(THIS_MODULE, "optical_sensors");
	if(IS_ERR(lpi->gp030_class)){
	  ret = PTR_ERR(lpi->gp030_class);
	  lpi->gp030_class = NULL;
	//  goto err_create_class;
        	}

	lpi->ls_dev = device_create(lpi->gp030_class, NULL, 0, "%s", "lightsensor");
	if(unlikely(IS_ERR(lpi->ls_dev))){
	  ret = PTR_ERR(lpi->ls_dev);
	  lpi->ls_dev = NULL;
	 // goto err_create_ls_device;
 	}

	ret =device_create_file(lpi->ls_dev, &dev_attr_ls_adc);
	if(ret){
	//  goto err_create_ls_device_file;
	}

 	printk(KERN_ALERT "driver loaded wan wan %d\n",lpi->test);
//err_create_singlethread_workqueue:

	return 0;
}

static int psals_init(void)
{
	psals_setup();
//	printk(KERN_ALERT "driver loaded wan wan wan\n");

	return 0;
}

static void psals_exit(void)
{
	struct gp030_info *lpi = lp_info;
	cancel_delayed_work_sync(&lpi->als_work);
//	device_remove_file(lpi->ls_dev);
	device_unregister(lpi->ls_dev); //do work
     	class_unregister(lpi->gp030_class);
//     	class_destroy(lpi->gp030_class);
//	device_destroy(lpi->gp030_class);
	printk(KERN_ALERT "derver unloaded\n");
}


module_init(psals_init); 
module_exit(psals_exit);

MODULE_LICENSE("Dual BSD/GPL");

