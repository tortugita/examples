#include <linux/init.h>
#include <linux/module.h>

static int hello_init(void)
{
  printk(KERN_ALERT "TEST: Hello from kernel module %s\n", __FUNCTION__);
  return 0;
} 

static void hello_exit(void)
{
  printk(KERN_ALERT "TEST: Bye from kernel module %s\n", __FUNCTION__);
} 

module_init(hello_init); 
module_exit(hello_exit); 
