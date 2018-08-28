#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

static LIST_HEAD(head0);
static LIST_HEAD(head1);
static LIST_HEAD(head2);

struct my_object {
  struct list_head list0;
  struct list_head list1;
  struct list_head list2;
  int sec;
};

int init_module(void)
{
  struct list_head *p;
  struct my_object *myobj;
  int i;

  for (i = 0; i < 10; ++i) {
    myobj = kmalloc(sizeof(struct my_object), GFP_KERNEL);
    myobj->sec = i;
   	list_add_tail(&myobj->list0, &head0);
	if (i % 2) {
    	list_add_tail(&myobj->list1, &head1);
	}
	else {
    	list_add_tail(&myobj->list2, &head2);
	}
  }

  list_for_each(p, &head1) {
    myobj = list_entry(p, struct my_object, list1);
    printk("odd %d\n", myobj->sec);
  }

  list_for_each(p, &head2) {
    myobj = list_entry(p, struct my_object, list2);
    printk("even %d\n", myobj->sec);
  }
  return 0;
}

void cleanup_module(void)
{
  struct my_object *myobj;
  while(!list_empty(&head0)) {
    myobj = list_entry(head0.next, struct my_object, list0);
    printk("all %d\n", myobj->sec);
    list_del(&myobj->list0);
    kfree(myobj);
  }
}

