#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

static int hello_init(void)
{
  struct task_struct *task;
  for_each_process(task)
  {  
    printk(KERN_ALERT "TEST: Hello from kernel module %s\n", __FUNCTION__);
    printk(KERN_INFO "TEST: process is %s and pid is %d\n", task->comm, task->pid);
  }
  return 0;
} 

module_init(hello_init); 
