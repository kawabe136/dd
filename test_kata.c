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
#include <asm/uaccess.h>
#include <asm/atomic.h>




struct struct_info{
	int temp1;
	int temp2;
	int temp3;
   	int set_data;
   	int get_data;

	atomic_t   delay;

	unsigned long enable;

};

static struct struct_info  *st_info ;

static int test_ioctl_setup(void);

static int test_ioctl_setup()
{
	int ret ;
	int sub_os_data[4] = {0, 0, 0, 0};
	signed int data_x = 0, data_y = 0;
	signed int data_z = 0;
 	int num;
	signed int ratio_x = 0, ratio_y = 0;
	ret = 0;

	sub_os_data[0] = 2238; 
	sub_os_data[1] = 2166; 
	sub_os_data[2] = 1500; 
	sub_os_data[3] = 1456; 

	data_y = sub_os_data[2] + sub_os_data[3] - sub_os_data[0] -sub_os_data[1];
	data_x = sub_os_data[1] + sub_os_data[2] - sub_os_data[0] -sub_os_data[3];
	
	for(num=0; num<4; num++)
	{
	  data_z += sub_os_data[num];
	}

	if(data_z == 0){
		ratio_y = 0;
		ratio_x = 0;
	}else{
		ratio_y = data_y / data_z;
		ratio_x = data_x / data_z;
	}

	printk(KERN_INFO "sub_os_data [0]:%d, [1]:%d, [2]:%d, [3]:%d \n",sub_os_data[0],sub_os_data[1], sub_os_data[2], sub_os_data[3]);
	printk(KERN_INFO "data_x :%d, data_y :%d, data_z:%d \n",data_x, data_y, data_z);
	printk(KERN_INFO "ratio_x :%d, ratio_y :%d \n",ratio_x, ratio_y);



	return ret;
}

static int test_ioctl_init(void)
{
	int ret;

	st_info = kzalloc( sizeof( struct struct_info ), GFP_KERNEL );
	ret = test_ioctl_setup( );
	printk(KERN_INFO "test_ioctl driver loaded \n");

	return 0;
}

static void test_ioctl_exit(void)
{

	kfree(st_info);
	printk(KERN_INFO "derver unloaded\n");
}


module_init(test_ioctl_init); 
module_exit(test_ioctl_exit);

MODULE_LICENSE("GPL");


