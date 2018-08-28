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


struct struct_data{
  int  			test;
};

static struct struct_data*   g_data_po;


static int dd_init(void)
{
    int ret = 0;
    struct struct_data *data;

    data = kzalloc(sizeof(struct struct_data), GFP_KERNEL);
    if (!data){
        return -ENOMEM;
    }

    data->test = 333;

    g_data_po = data;

    printk(KERN_ALERT "dd hello world:%d\n",data->test);

	printk(KERN_ALERT "dd_hello driver loaded \n");

	return ret;
}

static void dd_exit(void)
{
	printk(KERN_ALERT "dd_hello derver unloaded\n");
}


module_init(dd_init); 
module_exit(dd_exit);

MODULE_AUTHOR( "Isamu Kawabe <ikwb136@icloud.com>" ) ;
MODULE_DESCRIPTION( "TEST" ) ;
MODULE_LICENSE( "GPL" ) ;

