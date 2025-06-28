#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DRIVER_AUTHOR "hungle le771485@gmail.com"
#define DRIVER_DESC   "LED char device driver"
#define DRIVER_VERS   "1.0"

#define DEVICE_NAME "led_cdev"
#define CLASS_NAME  "led_class"
#define BUF_SIZE    32

struct led_dev {
    char *buffer;
    dev_t dev_num;
    struct class *dev_class;
    struct cdev cdev;
} led;

static int m_open(struct inode *inode, struct file *file) {
    pr_info("led_cdev: Device opened\n");
    return 0;
}

static int m_release(struct inode *inode, struct file *file) {
    pr_info("led_cdev: Device closed\n");
    return 0;
}

static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset) {
    size_t to_copy;

    pr_info("led_cdev: write() called\n");

    to_copy = (size > BUF_SIZE - 1) ? BUF_SIZE - 1 : size;
    memset(led.buffer, 0, BUF_SIZE);

    if (copy_from_user(led.buffer, user_buf, to_copy))
        return -EFAULT;

    led.buffer[to_copy] = '\0';  

    if (strncmp(led.buffer, "on", 2) == 0)
        pr_info("turn led on\n");
    else if (strncmp(led.buffer, "off", 3) == 0)
        pr_info("turn led off\n");
    else
        pr_info("Unknown command: %s\n", led.buffer);

    return size;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = m_open,
    .release = m_release,
    .write = m_write,
};

static int __init led_init(void) {
    if (alloc_chrdev_region(&led.dev_num, 0, 1, DEVICE_NAME) < 0) {
        pr_err("Failed to allocate device number\n");
        return -1;
    }

    led.dev_class = class_create(CLASS_NAME);
    if (IS_ERR(led.dev_class)) {
        pr_err("Failed to create device class\n");
        goto unregister_dev;
    }

    if (device_create(led.dev_class, NULL, led.dev_num, NULL, DEVICE_NAME) == NULL) {
        pr_err("Failed to create device\n");
        goto destroy_class;
    }

    cdev_init(&led.cdev, &fops);
    if (cdev_add(&led.cdev, led.dev_num, 1) < 0) {
        pr_err("Failed to add cdev\n");
        goto destroy_device;
    }

    led.buffer = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!led.buffer) {
        pr_err("Failed to allocate memory\n");
        goto del_cdev;
    }

    pr_info("led_cdev: Module loaded\n");
    return 0;

del_cdev:
    cdev_del(&led.cdev);
destroy_device:
    device_destroy(led.dev_class, led.dev_num);
destroy_class:
    class_destroy(led.dev_class);
unregister_dev:
    unregister_chrdev_region(led.dev_num, 1);
    return -1;
}

static void __exit led_exit(void) {
    kfree(led.buffer);
    cdev_del(&led.cdev);
    device_destroy(led.dev_class, led.dev_num);
    class_destroy(led.dev_class);
    unregister_chrdev_region(led.dev_num, 1);
    pr_info("led_cdev: Module unloaded\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);
