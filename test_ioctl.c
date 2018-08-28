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

#define ENABLE 1
#define SET_DATA 4
#define GET_DATA 5
#define DISENABLE 3

static int test_ioctl_setup(void);

struct struct_info{
	int temp1;
	int temp2;
	int temp3;
   	int set_data;
   	int get_data;
};

static struct struct_info  *st_info ;

static int test_open(struct inode *inode, struct file *file)
{

	printk(KERN_INFO "test_open \n");
	
	return 0;
}

static int test_release(struct inode *inode, struct file *file)
{

        printk(KERN_INFO "test_release \n");

        return 0;
}

static long test_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

	long err = 0;
	void __user *ptr = (void __user*) arg;
	uint32_t set_data;
	int get_data;
        printk(KERN_INFO "ioctl \n");
//	int ptr ;
//	ptr = 123;
	switch (cmd)
	{
	  case ENABLE:
		printk(KERN_INFO "ioctl ENABLE    cmd:%d \n",cmd);
 		break;
	  case DISENABLE:
		printk(KERN_INFO "ioctl DISENABLE cmd:%d \n",cmd);
 		break;
	  case SET_DATA:
		if(copy_from_user(&set_data, ptr, sizeof(set_data)))
		  {
		    err = -EFAULT;
		    goto err_out;
		  }
		st_info->set_data = set_data;
		printk(KERN_INFO "ioctl SET_DATA  cmd:%d set_data:%d\n",cmd, set_data);
 		break;
	  case GET_DATA:
		get_data = st_info->set_data * 2 ;
		if(copy_to_user(ptr, &get_data, sizeof(get_data)))
		  {
		    err = -EFAULT;
		    goto err_out;
		  }
		printk(KERN_INFO "ioctl GET_DATA  cmd:%d get_data:%d\n",cmd, get_data);
 		break;
	  default:
		printk(KERN_INFO "ioctl DEFAULT   cmd:%d \n",cmd);
 		break;
	}

//        printk(KERN_INFO "test_ioctl %d \n",st_info->temp3);
	err_out:
        return err;
}

static struct file_operations test_fops = {
	.owner   = THIS_MODULE,
	.open    = test_open,
	.release = test_release,
	.unlocked_ioctl = test_unlocked_ioctl,
};

static struct miscdevice test_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "test_ioctl",
	.fops = &test_fops
};

//static int test_ioctl_setup(struct struct_info *data)
static int test_ioctl_setup()
{
	int ret;
	ret = 0;
	st_info->temp1 = 100;
	st_info->temp2 = 200;
	st_info->temp3 = 300;
	printk(KERN_INFO "st_info.temp1:%d\n",st_info->temp1);

	ret = misc_register(&test_misc);
	if(ret < 0){
	  printk(KERN_ERR "driver file  error %d\n", ret);
	}
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
 	int err;

	if(( err = misc_deregister(&test_misc) ));
	{
	  printk(KERN_ALERT "misc_deregister fail: %d\n", err);
	}
	kfree(st_info);
	printk(KERN_INFO "derver unloaded\n");
}


module_init(test_ioctl_init); 
module_exit(test_ioctl_exit);

MODULE_LICENSE("GPL");


