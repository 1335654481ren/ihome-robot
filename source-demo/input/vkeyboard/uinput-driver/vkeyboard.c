#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    
#include <linux/aio.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/input.h>
int vkeyboard_major = 201;
module_param(vkeyboard_major, int, 0);
MODULE_AUTHOR("Renxl <liangxiao.ren@qq.com>");
MODULE_LICENSE("GPL");
 
static struct input_dev* vkeyboard_idev = NULL;
 
int vkeyboard_open(struct inode* inode, struct file* filp)
{
    return 0;
}
 
int vkeyboard_release(struct inode* inode, struct file* filp)
{
    return 0;
}
 
ssize_t vkeyboard_read(struct file* filp, char __user *buf, size_t count, loff_t* f_pos)
{
    printk(KERN_INFO"%s/n", __func__);
    return count;
}
 
struct keyboard_event
{
    int press;
    int key;
};
 
ssize_t vkeyboard_write(struct file* filp, const char __user * buf, size_t count, loff_t* f_pos)
{
    int ret = 0;
    struct keyboard_event event;
 
    while(ret < count)
    {
        if(copy_from_user(&event, buf + ret, sizeof(event)))
        {
            return -EFAULT;
        }
        ret += sizeof(event);
 
        input_event(vkeyboard_idev, EV_MSC, MSC_SCAN, event.key);
        input_report_key(vkeyboard_idev, event.key, event.press);
        input_sync (vkeyboard_idev);
 
        printk(KERN_INFO"%s p=%d key=%d with scan code/n", __func__, event.press, event.key);
    }
 
    return ret;
}
 

static int vkeyboard_input_dev_open(struct input_dev* idev)
{
    printk(KERN_INFO"%s/n", __func__);
 
    return 0;
}
 
static void vkeyboard_input_dev_close(struct input_dev* idev)
{
    printk(KERN_INFO"%s/n", __func__);
 
    return;
}

static int vkeyboard_input_dev_setup(void)
{
    int ret = 0;
    int i = 0;
    vkeyboard_idev = input_allocate_device();
 
    if(vkeyboard_idev == NULL)
    {
        return -ENOMEM;
    }
 
    vkeyboard_idev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) | BIT_MASK(EV_MSC);
    vkeyboard_idev->mscbit[0] = BIT_MASK(MSC_SCAN) | BIT_MASK(MSC_SERIAL) | BIT_MASK(MSC_RAW);
 
    bitmap_fill(vkeyboard_idev->keybit, KEY_MAX);
    bitmap_fill(vkeyboard_idev->relbit, REL_MAX);
    bitmap_fill(vkeyboard_idev->absbit, ABS_MAX);                
 
    vkeyboard_idev->name = "vkeyboard";
    vkeyboard_idev->phys = "vkeyboard/input0";
    vkeyboard_idev->open = vkeyboard_input_dev_open;
    vkeyboard_idev->close = vkeyboard_input_dev_close;
 
    for(i = 32; i < KEY_MAX; i++)
    {
        input_set_capability(vkeyboard_idev, EV_KEY, i);
    }
    __set_bit(EV_KEY, vkeyboard_idev->evbit);
 
    ret = input_register_device(vkeyboard_idev);
 
    return ret;
}
 
static struct file_operations vkeyboard_fops = 
{
    .owner = THIS_MODULE,
    .open    = vkeyboard_open,
    .release = vkeyboard_release,
    .read    = vkeyboard_read,
    .write   = vkeyboard_write,
};
 
static int __init vkeyboard_init(void)
{
    int result = register_chrdev(vkeyboard_major, "vkeyboard", &vkeyboard_fops);
    vkeyboard_input_dev_setup();
 
    return result;
}
 
static void __exit vkeyboard_cleanup(void)
{
    input_unregister_device(vkeyboard_idev);
    unregister_chrdev(vkeyboard_major, "vkeyboard");
 
    return;
}
 
module_init(vkeyboard_init);
module_exit(vkeyboard_cleanup);


