#include <linux/module.h>       /* MODULE, THIS_MODULE */
#include <linux/moduleparam.h>
#include <linux/kernel.h>       /* printk() */
#include <linux/errno.h>	    /* error codes */
#include <linux/list.h>         /* list_*() */
#include <linux/slab.h>         /* kmalloc() */


#ifndef LISTTEST_NR_DEVS
#define LISTTEST_NR_DEVS 4 /* マイナーデバイス番号の総数 */
#endif

#ifndef DRIVER_NAME
#define DRIVER_NAME "hello"
#endif

int listtest_major = 0;
int listtest_minor = 0;
int listtest_nr_devs = LISTTEST_NR_DEVS;

struct test_data
{
    int no;
    struct list_head list;
};
 
struct test_data head = {
    .no = -0,
};
 

#if 0
static void listtest_push_no(int no)
{
    struct test_data *new_data;

    new_data = kmalloc(sizeof(struct test_data), GFP_KERNEL);
    if(new_data) {
        new_data->no = no;
        list_add(&new_data->list, &head.list);
    }
}
#endif
 

static void listtest_show_list(void)
{
    struct list_head *listptr;
    struct test_data *entry;

    printk(KERN_ALERT "show_list\n");

    printk(KERN_ALERT "no = %d (list %p, prev = %p, next = %p)\n", 
           head.no, &head.list, head.list.prev, head.list.next);
 
    list_for_each(listptr, &head.list) {
        entry = list_entry(listptr, struct test_data, list);
        printk(KERN_ALERT "no = %d (list %p, prev = %p, next = %p)\n", 
               entry->no, &entry->list, entry->list.prev, entry->list.next);
    }
}
 

#if 0
static void listtest_pop(void)
{
    struct test_data *ptr;

    ptr = list_entry(head.list.next, struct test_data, list);
    list_del(head.list.next);

    kfree(ptr);
}
#endif
 

static int listtest_init(void)
{
    struct test_data *ptr;
    struct test_data *new_data;
    struct test_data *new_data2;
    int i;


    INIT_LIST_HEAD(&head.list);

    listtest_show_list();

    for(i=1;i<5;i++){
        new_data = kmalloc(sizeof(struct test_data), GFP_KERNEL);
        new_data->no = i;
        list_add_tail(&new_data->list, &head.list);
    }

    listtest_show_list();

    ptr = list_entry(head.list.next, struct test_data, list);

    list_del(head.list.next);

    listtest_show_list();

    kfree(ptr);

    return 0;
}

 

#if 0
static int listtest_init(void)
{
    int i;
    printk(KERN_ALERT "Listtest init\n");

    INIT_LIST_HEAD(&head.list);

    for(i = 0; i < 3; ++i) {
        listtest_push_no(i);
        listtest_show_list();
    }

    for(i = 0; i < 3; ++i) {
        listtest_pop();
        listtest_show_list();
    }

    return 0;
}
#endif

static void listtest_exit(void)
{
    printk(KERN_ALERT "Listtest exit\n");
}
 

module_init(listtest_init);
module_exit(listtest_exit);

MODULE_AUTHOR("Isama Kawabe");
MODULE_LICENSE("Dual BSD/GPL");

