#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/scatterlist.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

struct workqueue_struct *test_wq;
struct delayed_work test_dwq;
void delay_func(struct work_struct *work) {
    int i;

    printk(KERN_EMERG "My name is delay_func!\n");
    for (i = 0; i < 3; i++) {
        printk(KERN_ERR "delay_fun:i=%d\n", i);
        msleep(1000);
    }
}
static int __init delaywork_init(void) {
	int i;
	int ret;
	test_wq = create_workqueue("test_wq");
	if(!test_wq) {
		printk(KERN_EMERG "no memory for work queue\n");
		return 1;
	}
	printk(KERN_EMERG "create work queue seccessful!\n");

	INIT_DELAYED_WORK(&test_dwq,delay_func);

	ret = mod_delayed_work(test_wq,&test_dwq,1000);
	printk(KERN_EMERG "first exec work ret=%d\n",ret);

	for(i=0; i < 3; i++) {
		printk(KERN_EMERG "start cycle get work status ret=%d i=%d \n",ret,i);
		msleep(100);
	}
	ret = mod_delayed_work(test_wq,&test_dwq,1000);
	printk(KERN_EMERG "second exec work ret=%d\n",ret);

	return 0;
}

static void __exit delaywork_exit(void) {
	int ret ;
	ret = cancel_delayed_work(&test_dwq);
	flush_workqueue(test_wq);
	destroy_workqueue(test_wq);
	printk("good bay !\n");
}

module_init(delaywork_init);
module_exit(delaywork_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("test work queue");
MODULE_AUTHOR("yuelin.tang");
