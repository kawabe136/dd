#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/jiffies.h>

MODULE_AUTHOR("y.kitamura");
MODULE_DESCRIPTION("kthread test");
MODULE_LICENSE("GPL");

static struct task_struct *kthread_tsk;

wait_queue_head_t poll_thread_wq;

static int poll_thread_exit = 0;

static void my_kthread_main(void)
{
       set_current_state(TASK_INTERRUPTIBLE);
       schedule_timeout(125);
       printk("kthread:%ld\n", jiffies);
}

static int my_kthread(void *arg)
{
       printk("HZ:%d\n", HZ);
       while (!kthread_should_stop()) {
			wait_event(poll_thread_wq,poll_thread_exit);
               my_kthread_main();
       }
       return 0;
}

static int __init kthread_test_init(void)
{

	   init_waitqueue_head(&poll_thread_wq);
       kthread_tsk = kthread_run(my_kthread, NULL, "param %s&%d", "test", 123);
       if (IS_ERR(kthread_tsk))
               return -1;
       printk("pid->%d:prio->%d:comm->%s\n",
               kthread_tsk->pid,
               kthread_tsk->static_prio,
               kthread_tsk->comm);

	   wake_up(&poll_thread_wq);

       return 0;
}

static void __exit kthread_test_exit(void)
{
	poll_thread_exit = 1;
       kthread_stop(kthread_tsk);
}

module_init(kthread_test_init);
module_exit(kthread_test_exit);
