#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include "ioctl_basic.h"

#define UNLOCKED_IOCTL //for linux version >2.6.36
static int Major;

int open(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "eestream debug: open\n");
    return 0;
}


int release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "eestream debug: release\n");
    return 0;
}


#ifndef UNLOCKED_IOCTL
int ioctl_func(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;

    switch(cmd)
    {
        case IOCTL_HELLO:
            printk(KERN_INFO "eestream debug:%s, Hello ioctlworld\n", __FUNCTION__);
            break;
    }

    return ret;
}
#else
long unlocked_ioctl_func(struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct inode *inode = file_inode(filp);
    int ret = 0;

    switch(cmd)
    {
        case IOCTL_HELLO:
            printk(KERN_INFO "eestream debug:%s, before loop\n", __FUNCTION__);
            while(1);
            printk(KERN_INFO "eestream debug: %s, after loop\n", __FUNCTION__);
            break;
    }

    return ret;
}
#endif

struct file_operations fops = {
open: open,
#ifndef UNLOCKED_IOCTL
ioctl: ioctl_func,
#else
unlocked_ioctl: unlocked_ioctl_func,
#endif
release: release
};

struct cdev *kernel_cdev;

int char_arr_init(void){
    int ret;
    dev_t dev_no, dev;

    kernel_cdev = cdev_alloc();
    kernel_cdev->ops = &fops;
    kernel_cdev->owner = THIS_MODULE;

    printk("eestream debug: init\n");

    ret = alloc_chrdev_region( &dev_no, 0, 1, "char_arr_dev");
    if(ret<0){
        printk("fail\n");
        return ret;
    }

    Major = MAJOR(dev_no);
    dev = MKDEV(Major, 0);
    printk("eestream debug: The major number for device is %d\n", Major);

    ret = cdev_add(kernel_cdev, dev, 1);
    if(ret<0)
    {
        printk("Unable to allocate dev\n");
        return ret;
    }

    return 0;
}


void char_arr_cleanup(void){
    printk("eestream debug: cleanup\n");
    cdev_del(kernel_cdev);
    unregister_chrdev_region(Major, 1);
}

MODULE_LICENSE("GPL");
module_init(char_arr_init);
module_exit(char_arr_cleanup);

