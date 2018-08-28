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

/*
 * ioctlのcmd番号をマクロを使って設定する
 * - 番号のbaseとなるマジック番号を 'k'とする
 * - 引数なしのコマンド用 _IO(type,number)
 * - データ読み出し用 _IOR(type,number,datatype)
 * - データ書き込み用 _IOW(type,number,datatype)
 * - 双方向転送用     _IOWR(type,number,datatype)

 Device file is created in the /dev.
 $ /dev/dd_misc
 You have to change the permissions on the device file using the chmod command.
 Run the check program.

 */


#define IKOMA_MAGIC 'k'
#define IKOMA_IOCTL_ENABLE				_IO(IKOMA_MAGIC, 	0x01)
#define IKOMA_IOCTL_GET_DATA 			_IOR(IKOMA_MAGIC, 	0x02, int)
#define IKOMA_IOCTL_SET_DATA			_IOW(IKOMA_MAGIC, 	0x03, int)
#define IKOMA_IOCTL_DISABLE				_IO(IKOMA_MAGIC, 	0x04)


struct struct_data{
    int opend;
    int enabled;
   	int set_data;
   	int get_data;
};

static struct struct_data  *st_data ;

static int dd_open(struct inode *inode, struct file *file)
{

    st_data->opend = 1;
	printk(KERN_INFO "dd_misc_open \n");
	
	return 0;
}

static int dd_release(struct inode *inode, struct file *file)
{

    st_data->opend = 0;
    printk(KERN_INFO "dd_misc_release \n");

        return 0;
}

static long dd_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

	long err = 0;
	void __user *ptr = (void __user*) arg;
	uint32_t set_data;
	int get_data;

	switch (cmd)
	{
	  case IKOMA_IOCTL_ENABLE:
		printk(KERN_INFO "ioctl ENABLE cmd:%d \n",cmd);
 		break;
	  case IKOMA_IOCTL_DISABLE:
		printk(KERN_INFO "ioctl DISABLE cmd:%d \n",cmd);
 		break;
	  case IKOMA_IOCTL_SET_DATA:
		if(copy_from_user(&set_data, ptr, sizeof(set_data)))
		  {
		    err = -EFAULT;
		    goto err_out;
		  }
		st_data->set_data = set_data;
		printk(KERN_INFO "ioctl SET_DATA  cmd:%d set_data:%d\n",cmd, set_data);
 		break;
	  case IKOMA_IOCTL_GET_DATA:
		get_data = st_data->set_data * 2 ;
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

	err_out:
        return err;
}

static struct file_operations dd_fops = {
	.owner   = THIS_MODULE,
	.open    = dd_open,
	.release = dd_release,
	.unlocked_ioctl = dd_unlocked_ioctl,
};

static struct miscdevice dd_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dd_misc",
	.fops = &dd_fops
};


static int dd_init(void)
{
	int ret;
    struct struct_data *data;

	//printk(KERN_INFO "%s\n",__func__);
	data = kzalloc( sizeof( struct struct_data ), GFP_KERNEL );
	if( !data )
	{
		printk( KERN_ERR "kzalloc error !! \n" ) ;
		return -ENOMEM ;
	}

    st_data = data;

    ret = misc_register(&dd_misc);
    if(ret < 0){
      printk(KERN_ERR "driver file  error %d\n", ret);
    }

	printk(KERN_INFO "dd_misc driver loaded \n");

	return 0;
}

static void dd_exit(void)
{
 	int err;

	if(( err = misc_deregister(&dd_misc) ));
	{
	  printk(KERN_ALERT "misc_deregister fail: %d\n", err);
	}
	kfree(st_data);
	printk(KERN_INFO "derver unloaded\n");
}


module_init(dd_init); 
module_exit(dd_exit);

MODULE_AUTHOR( "Isamu Kawabe <ikwb136@icloud.com>" ) ;
MODULE_DESCRIPTION( "TEST" ) ;
MODULE_LICENSE( "GPL" ) ;


