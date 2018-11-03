#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DRIVER_AUTHOR "Beat Lussi"
#define DRIVER_DESC "Testmodule with an argument"
#define SUCCESS 0
#define DEVICE_NAME "lube"
#define DEVICE_COUNT 1
#define BUF_LEN 80

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("lube");

/*
 * Prototypes
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

/*
 * Global variables
 */
static int major;
static int minor;
static dev_t dev;
static struct cdev *my_cdev;
static struct class *my_class;
static int deviceOpen = 0;
static char msg[BUF_LEN];
static char *msg_ptr;
static struct file_operations fops = {
  .owner = THIS_MODULE,
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};

int init_module(void)
{
  int ret = 0;
  printk(KERN_INFO "TEST: Init\n");
  
  // If major is not known -> allocation with alloc_chrdev_region
  // It registers in /proc/devces and sysfs
  ret = alloc_chrdev_region(&dev, 0, DEVICE_COUNT, DEVICE_NAME);
  if (ret < 0) {
    printk(KERN_ALERT "TEST: Allocating major numbers failed with %d\n", ret);
  }
  
  // If major is not known
  major = MAJOR(dev);
  // If minor is not known
  minor = MINOR(dev);
  printk(KERN_INFO "TEST: The major number is %d and the minor is %d\n", major, minor);
  
  // If major is known -> register it with register_chrdev_region
  // It registers in /proc/devices and sysfs
  // ret = register_chrdev_region(dev, DEVICE_COUNT, DEVICE_NAME);
  // if (ret < 0) {
  //   printk(KERN_ALERT "TEST: Registering char device failed with %d\n", ret);
  // }
  
  // If major and minor are known and the dev not
  // dev = MKDEV(major, minor);

  // register_chrdev ist old use -> 
  // major = register_chrdev(0, DEVICE_NAME, &fops);
  
  if (major < 0)
  {
    printk(KERN_ALERT "TEST: Registering char device failed with %d\n", major);
    return major;
  }
 
  // Create class 
  my_class = class_create(THIS_MODULE, "lube");
  if (!my_class)
  {
    printk(KERN_ALERT "TEST: Creating class failed\n");
  }
 
  // Create device in /dev 
  if (!device_create(my_class, NULL, dev, NULL, "lube%d", MINOR(dev)))
  {
    printk(KERN_ALERT "TEST: Creating device failed\n");
  } 
  
  // Initialize the char device cdev
  my_cdev = cdev_alloc();
  cdev_init(my_cdev, &fops);
  ret = cdev_add(my_cdev, dev, 1);
  if (ret < 0) {
    printk(KERN_ALERT "TEST: Adding cdev failed with %d\n", ret);
  }
  
  printk(KERN_INFO "TEST: Registering char device successfull with %d\n", major);
  printk(KERN_INFO "TEST: 'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
  return SUCCESS;
} 

void cleanup_module(void)
{
  printk(KERN_INFO "TEST: Bye\n");
  cdev_del(my_cdev);
  device_destroy(my_class, dev);
  class_destroy(my_class);
  unregister_chrdev_region(dev, DEVICE_COUNT);
  // Old version -> unregister_chrdev(major, DEVICE_NAME);
  
}

static int device_open(struct inode *inode, struct file *file)
{
  static int counter = 0;
  if (deviceOpen)
  {
    return -EBUSY; 
  }
  deviceOpen++;
  sprintf(msg, "I am already open %d times\n", counter++);
  msg_ptr = msg;
  try_module_get(THIS_MODULE);
  
  return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
   deviceOpen--; 
   module_put(THIS_MODULE);
   return 0;
}

static ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset)
{
  int bytes_read = 0;
  if (*msg_ptr == 0)
  {
    return 0; 
  }
  
  while (length && *msg_ptr)
  {
    put_user(*(msg_ptr++), buffer++);
    length--;
    bytes_read++;
  }
  return bytes_read;
}

static ssize_t device_write(struct file *file, const char *buff, size_t len, loff_t *off)
{
  printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
  return -EINVAL;
}

