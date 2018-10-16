#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>

int focus_time_interval = 5000;
struct timer_list focus_timer;

static void focus_timer_handler(unsigned long data)
{
  /* restarting timer */
  mod_timer(&focus_timer, jiffies + msecs_to_jiffies(focus_time_interval));
  printk(KERN_INFO "FOCUS: Timer handler called\n");
}

static int focus_timer_init(void)
{
  setup_timer(&focus_timer, focus_timer_handler, 0);
  mod_timer(&focus_timer, jiffies + msecs_to_jiffies(focus_time_interval));
  printk(KERN_INFO "FOCUS: Hello from kernel module timer\n");
  return 0;
} 

static void focus_timer_exit(void)
{
  del_timer(&focus_timer);
  printk(KERN_INFO "FOCUS: Bye from kernel module timer\n");
} 

module_init(focus_timer_init); 
module_exit(focus_timer_exit); 
