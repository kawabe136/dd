#include <linux/module.h>       /* MODULE macro, THIS_MODULE */
#include <linux/moduleparam.h>
#include <linux/kernel.h>       /* printk() */
#include <linux/fs.h>			/* alloc_chrdev_region(), ... */
#include <linux/types.h>        /* dev_t */
#include <linux/kdev_t.h>       /* MAJOR() */
/* #include <linux/errno.h>		/\* error codes *\/ */
#include <linux/cdev.h>         /* cdev_*() */
/* #include <linux/sched.h>        /\* current *\/ */
#include <linux/stat.h>         /* S_IRUGO, S_IWUSR */
#include <linux/slab.h>         /* kmalloc() */

inline static int ikoma_setup(void)
{
    struct device* laser_device = NULL;

    LOG_INF("Start\n");

    //Allocate char driver no.
    if( alloc_chrdev_region(&g_Laser_devno, 0, 1,LASER_DRVNAME) )
    {
        LOG_INF("Allocate device no failed\n");

        return -EAGAIN;
    }

    //Allocate driver
    g_pLaser_CharDrv = cdev_alloc();

    if(NULL == g_pLaser_CharDrv)
    {
        unregister_chrdev_region(g_Laser_devno, 1);

        LOG_INF("Allocate mem for kobject failed\n");

        return -ENOMEM;
    }

    //Attatch file operation.
    cdev_init(g_pLaser_CharDrv, &g_stLaser_fops);

    g_pLaser_CharDrv->owner = THIS_MODULE;

    //Add to system
    if(cdev_add(g_pLaser_CharDrv, g_Laser_devno, 1))
    {
        LOG_INF("Attatch file operation failed\n");

        unregister_chrdev_region(g_Laser_devno, 1);

        return -EAGAIN;
    }

    actuator_class = class_create(THIS_MODULE, LASER_DRIVER_CLASS_NAME);
    if (IS_ERR(actuator_class)) {
        int ret = PTR_ERR(actuator_class);
        LOG_INF("Unable to create class, err = %d\n", ret);
        return ret;
    }

    laser_device = device_create(actuator_class, NULL, g_Laser_devno, NULL, LASER_DRVNAME);

    if(NULL == laser_device)
    {
        return -EIO;
    }

    device_create_file(laser_device, &dev_attr_ctrl);
    meizu_sysfslink_register_n(laser_device, "laser");
    LOG_INF("End\n");
    return 0;
}


static int ikoma_init(void)
{
    printk(KERN_INFO "****************************** \n");
    printk(KERN_INFO "   sysfs test driver loaded\n");
    printk(KERN_INFO "****************************** \n");

    ikoma_setup();

    return 0;
}

static void ikoma_exit(void)
{
    struct test_info *infop = lp_info;

    printk(KERN_INFO "******************************** \n");
    printk(KERN_INFO "   sysfs test driver unloaded\n");
    printk(KERN_INFO "******************************** \n");

//  device_remove_file(infop->test_dev);
    device_unregister(infop->test_dev); //do work
    class_unregister(infop->test_class);
    kfree(infop);
//      class_destroy(infop->test_class);
//  device_destroy(infop->test_class);
    printk(KERN_ALERT "derver unloaded\n");
}


module_init(ikoma_init);
module_exit(ikoma_exit);


MODULE_AUTHOR( "Isamu Kawabe <ikwb136@icloud.com>" ) ;
MODULE_DESCRIPTION( "TEST" ) ;
MODULE_LICENSE( "GPL" ) ;

