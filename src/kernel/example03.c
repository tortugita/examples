#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>

#define DRIVER_AUTHOR "Beat Lussi"
#define DRIVER_DESC "Testmodule with an argument"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");

int myVar = 0;

static int hello_init(void)
{
  printk(KERN_INFO "TEST: Init, the variable myVar has the value %d\n", myVar);
  return 0;
} 

static void hello_cleanup(void)
{
  printk(KERN_INFO "TEST: Bye\n");
}

module_init(hello_init); 
module_exit(hello_cleanup);
module_param(myVar, int, 0);
