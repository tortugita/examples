#include <linux/init.h>
#include <linux/module.h>


static void hello_exit(void)
{
  printk(KERN_ALERT "TEST: Bye from kernel module %s\n", __FUNCTION__);
} 

module_exit(hello_exit); 
