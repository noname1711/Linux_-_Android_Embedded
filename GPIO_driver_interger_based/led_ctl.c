#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#define DEVICE_NAME "ledctl"
#define CLASS_NAME  "led_class"

#define GPIO_LED    529   //GPIO17 = base gpio + offset (512 + 17)      
#define BUF_LEN     8

static dev_t dev_num;
static struct cdev led_cdev;
static struct class *led_class;
static char kbuf[BUF_LEN];

static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    if (count > BUF_LEN - 1)
        return -EINVAL;

    if (copy_from_user(kbuf, buf, count))
        return -EFAULT;

    kbuf[count] = '\0';

    if (strncmp(kbuf, "on", 2) == 0) {
        pr_info("turn led on\n");
        gpio_set_value(GPIO_LED, 1);
    } else if (strncmp(kbuf, "off", 3) == 0) {
        pr_info("turn led off\n");
        gpio_set_value(GPIO_LED, 0);
    } else {
        pr_warn("Invalid command: %s\n", kbuf);
    }

    return count;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = led_write,
};

static int __init led_init(void)
{
    int ret;

    // Register char device number
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    // printf major number
    pr_info("led_ctl major = %d\n", MAJOR(dev_num));

    if (ret) {
        pr_err("Failed to allocate chrdev\n");
        return ret;
    }

    // Init char device
    cdev_init(&led_cdev, &fops);
    ret = cdev_add(&led_cdev, dev_num, 1);
    if (ret) {
        pr_err("Failed to add cdev\n");
        goto unregister_chrdev;
    }

    // Create device class
    led_class = class_create(CLASS_NAME);

    if (IS_ERR(led_class)) {
        pr_err("Failed to create class\n");
        ret = PTR_ERR(led_class);
        goto del_cdev;
    }

    // Create device node
    device_create(led_class, NULL, dev_num, NULL, DEVICE_NAME);

    // GPIO setup
    ret = gpio_request(GPIO_LED, "led_gpio");
    if (ret) {
        pr_err("Failed to request GPIO %d\n", GPIO_LED);
        goto destroy_device;
    }

    gpio_direction_output(GPIO_LED, 0); // Init LED OFF

    pr_info("led_ctl module loaded\n");
    return 0;

destroy_device:
    device_destroy(led_class, dev_num);
    class_destroy(led_class);
del_cdev:
    cdev_del(&led_cdev);
unregister_chrdev:
    unregister_chrdev_region(dev_num, 1);
    return ret;
}

static void __exit led_exit(void)
{
    gpio_set_value(GPIO_LED, 0); // Turn off before exit
    gpio_free(GPIO_LED);
    device_destroy(led_class, dev_num);
    class_destroy(led_class);
    cdev_del(&led_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("led_ctl module unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hungle le771485@gmail.com");
MODULE_DESCRIPTION("LED control via GPIO17 using integer GPIO API");
MODULE_VERSION("1.0");

module_init(led_init);
module_exit(led_exit);
