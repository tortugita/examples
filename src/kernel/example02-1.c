#include <linux/init.h>
#include <linux/module.h>

static int hello_init(void)
{
  printk(KERN_ALERT "TEST: Hello from kernel module %s\n", __FUNCTION__);
  return 0;
} 

module_init(hello_init); 
