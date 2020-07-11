#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

void fun5(void) {
    printk(KERN_INFO "sup: running %s\n", __func__);
}
void fun4(void) {
    printk(KERN_INFO "sup: running %s\n", __func__);
    fun5();
}
void fun3(void) {
    printk(KERN_INFO "sup: running %s\n", __func__);
    fun4();
}
void fun2(void) {
    printk(KERN_INFO "sup: running %s\n", __func__);
    fun3();
}
void fun1(void) {
    printk(KERN_INFO "sup: running %s\n", __func__);
    fun2();
}


static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world! %*c %sok\n", 10, 'M', __func__);
    printk(KERN_INFO "Hello world! %*c %sok\n", 20, 'K', __func__);
    // printk(KERN_ERR  "sup:1 %_*c %s ok\n", 10, 'K', __func__);
    printk(KERN_INFO "%*c\n", 20, 'A');
    fun1();
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

module_init(hello_init);
module_exit(hello_cleanup);



