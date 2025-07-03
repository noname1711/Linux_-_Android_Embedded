#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of.h>

#define DEVICE_NAME "myled"
#define CLASS_NAME  "ledclass"

static struct gpio_desc *gpio_led;
static struct class *led_class;
static struct cdev led_cdev;
static dev_t dev_num;

static const struct of_device_id led_dt_ids[] = {
    { .compatible = "gpio-descriptor-based" },
    { }
};

static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    char kbuf[10] = {0};

    if (copy_from_user(kbuf, buf, count))
        return -EFAULT;

    if (strncmp(kbuf, "on", 2) == 0) {
        gpiod_set_value(gpio_led, 1);
        pr_info("LED ON\n");
    } else if (strncmp(kbuf, "off", 3) == 0) {
        gpiod_set_value(gpio_led, 0);
        pr_info("LED OFF\n");
    } else {
        pr_warn("Unknown command: %s\n", kbuf);
    }

    return count;
}

static struct file_operations fops = {
    .owner  = THIS_MODULE,
    .write  = led_write,
};

static int my_pdrv_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;

    pr_info("Platform driver probed\n");

    gpio_led = gpiod_get(dev, "led27", GPIOD_OUT_LOW);
    if (IS_ERR(gpio_led)) {
        pr_err("Failed to get GPIO descriptor\n");
        return PTR_ERR(gpio_led);
    }

    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&led_cdev, &fops);
    cdev_add(&led_cdev, dev_num, 1);

    led_class = class_create(CLASS_NAME);
    device_create(led_class, NULL, dev_num, NULL, DEVICE_NAME);

    pr_info("LED driver ready (/dev/%s)\n", DEVICE_NAME);
    pr_info("%s  - %d \n", __func__, __LINE__);
    return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
    gpiod_set_value(gpio_led, 0); 

    device_destroy(led_class, dev_num);
    class_destroy(led_class);
    cdev_del(&led_cdev);
    unregister_chrdev_region(dev_num, 1);

    gpiod_put(gpio_led);

    pr_info("LED driver removed\n");
    pr_info("%s  - %d \n", __func__, __LINE__);
    return 0;
}

static struct platform_driver mypdrv = {
    .probe = my_pdrv_probe,
    .remove = my_pdrv_remove,
    .driver = {
        .name = "descriptor-based",
        .of_match_table = of_match_ptr(led_dt_ids),
        .owner = THIS_MODULE,
    },
};
module_platform_driver(mypdrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hungle le771485@gmail.com");
MODULE_DESCRIPTION("GPIO LED control via descriptor and userspace app");
MODULE_VERSION("1.0");
