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

/*
/linux-3.18.36/include/linux/types.h

typedef struct {
    int counter;
} atomic_t;

*/

typedef enum{
  CMC_BIT_ALS,
  CMC_BIT_PS,
}CMC_BIT;

struct struct_data{
  atomic_t       delay;
  unsigned long  enable;
};

static struct struct_data*   g_data_po;


static int dd_init(void)
{
    int ret = 0;
    int temp;
    struct struct_data *data;

    data = kzalloc(sizeof(struct struct_data), GFP_KERNEL);
    if (!data){
        return -ENOMEM;
    }

    g_data_po = data;

//atomic
	atomic_set(&data->delay, 0);
	temp = atomic_read(&data->delay);
	printk(KERN_INFO "data.delay:%d\n",temp);

	atomic_set(&data->delay, 50);
	temp = atomic_read(&data->delay);
	printk(KERN_INFO "data.delay:%d\n",temp);

	// add
	atomic_add(50, &data->delay);
	temp = atomic_read(&data->delay);
	printk(KERN_INFO "data.delay:%d\n",temp);

//bit
	set_bit(CMC_BIT_ALS, &data->enable);
	printk(KERN_INFO "data.enable:%lx\n",data->enable);

	set_bit(CMC_BIT_PS, &data->enable);
	printk(KERN_INFO "data.enable:%lx\n",data->enable);

	clear_bit(CMC_BIT_ALS, &data->enable);
	clear_bit(CMC_BIT_PS, &data->enable);
	printk(KERN_INFO "data.enable:%lx\n",data->enable);

	set_bit(CMC_BIT_ALS, &data->enable);
	temp = test_bit(CMC_BIT_ALS, &data->enable) ? (1) : (0) ;
	printk(KERN_INFO "test_bit:%d\n",temp);

	clear_bit(CMC_BIT_ALS, &data->enable);
	temp = test_bit(CMC_BIT_ALS, &data->enable) ? (1) : (0) ;
	printk(KERN_INFO "test_bit:%d\n",temp);

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

