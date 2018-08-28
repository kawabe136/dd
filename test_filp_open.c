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
#include <linux/fs.h>
#include <asm/uaccess.h>


#define ENABLE 1
#define SET_DATA 4
#define GET_DATA 5
#define DISENABLE 3

#define FILE_PATH "/home/kawabe/filpopen"


struct struct_info{
	int temp1;
	int temp2;
	int temp3;
   	int set_data;
   	int get_data;
};

static struct struct_info  *st_info ;

static int read_crosstalk(char *buf, int len)
{
    struct file *cal_filp = NULL;
    int err = 0;
    mm_segment_t old_fs;
   
	old_fs = get_fs();
	set_fs(KERNEL_DS);

    cal_filp = filp_open(FILE_PATH,
                         O_RDONLY,
                         S_IRUGO | S_IWUSR |S_IWGRP);

    if(IS_ERR(cal_filp)){
		printk(KERN_ERR "%s:Can't open file \n ",__func__);
		err = PTR_ERR(cal_filp);
        goto end;
	} 

    err = cal_filp->f_op->read(cal_filp,
                  buf, len - 1, &cal_filp->f_pos);
	buf[4] = '\0';

	if (err != len - 1){
    	printk(KERN_ERR "%s:Can't read data from file", __func__);
		err = -EIO;
        goto end;
 	}
	printk(KERN_INFO "%s:filp_open read value = %s \n ",__func__, buf );

    filp_close(cal_filp, current->files);
end:
    set_fs(old_fs);
    return err;

}

static int write_crosstalk(char *buf, int len)
{
    struct file *cal_filp = NULL;
    int err = 0;
    mm_segment_t old_fs;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    cal_filp = filp_open(FILE_PATH,
                         O_CREAT | O_TRUNC | O_WRONLY,
                         S_IRUGO | S_IWUSR |S_IWGRP);
    if(IS_ERR(cal_filp)){
        printk(KERN_ERR "%s:Can't open file \n ",__func__);
        err = PTR_ERR(cal_filp);
        goto end;
    }

    err = cal_filp->f_op->write(cal_filp, buf, len - 1, &cal_filp->f_pos);
	if (err != len - 1){
    	printk(KERN_ERR "%s:Can't write data to file", __func__);
		err = -EIO;
        goto end;
 	}

    filp_close(cal_filp, current->files);
end:
    set_fs(old_fs);
    return err;

}

static int test_open(struct inode *inode, struct file *file)
{

	printk(KERN_INFO "test_filp_open open\n");
	
	return 0;
}

static int test_release(struct inode *inode, struct file *file)
{

        printk(KERN_INFO "test_filp_open release \n");

        return 0;
}

static long test_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

	long err = 0;
	void __user *ptr = (void __user*) arg;
	uint32_t set_data;
	int get_data;
    char buf[5];
	printk(KERN_INFO "buf[5] size = %d \n ", sizeof(buf));

	switch (cmd)
	{
	  case ENABLE:
		printk(KERN_INFO "ioctl ENABLE    cmd:%d \n",cmd);

        read_crosstalk(buf, sizeof(buf));

		strcpy(buf,"4321");
        write_crosstalk(buf, sizeof(buf));

		//verify
        read_crosstalk(buf, sizeof(buf));

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


static int test_ioctl_init(void)
{
	int ret;

	st_info = kzalloc( sizeof( struct struct_info ), GFP_KERNEL );

    printk(KERN_INFO "********************************** \n");
    printk(KERN_INFO "  filp_open test driver loaded\n");
    printk(KERN_INFO "********************************** \n");

	ret = misc_register(&test_misc);
	if(ret < 0){
	  printk(KERN_ERR "driver file  error %d\n", ret);
	}
	return ret;
}

static void test_ioctl_exit(void)
{
 	int err;

	err = misc_deregister(&test_misc) ;

	if(err < 0){
	  printk(KERN_ALERT "misc_deregister fail: %d\n", err);
	}

	kfree(st_info);

    printk(KERN_INFO "********************************** \n");
    printk(KERN_INFO "  filp_open test driver unloaded\n");
    printk(KERN_INFO "********************************** \n");

}


module_init(test_ioctl_init); 
module_exit(test_ioctl_exit);

MODULE_LICENSE("GPL");

 
