#include <linux/module.h>
#include <linux/io.h>
#include "led.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hungle le771485@gmail.com");
MODULE_DESCRIPTION("GPIO Control Functions");

static void __iomem *gpio_base;

void gpio_setup(void) {
    uint32_t reg;

    gpio_base = ioremap(BCM2835_GPIO_BASE_ADDR, 0x28);
    if (!gpio_base) {
        pr_err("Failed to map GPIO\n");
        return;
    }

    reg = ioread32(gpio_base + GPIO_FSEL_OFFSET + (GPIO_NUMBER_27 / 10) * 4);
    reg &= ~(7 << ((GPIO_NUMBER_27 % 10) * 3));     // clear bits
    reg |= (1 << ((GPIO_NUMBER_27 % 10) * 3));      // set output
    iowrite32(reg, gpio_base + GPIO_FSEL_OFFSET + (GPIO_NUMBER_27 / 10) * 4);
}
EXPORT_SYMBOL(gpio_setup);

void gpio_set_high(void) {
    if (gpio_base)
        iowrite32(1 << GPIO_NUMBER_27, gpio_base + GPIO_SET_0_OFFSET);
}
EXPORT_SYMBOL(gpio_set_high);

void gpio_set_low(void) {
    if (gpio_base)
        iowrite32(1 << GPIO_NUMBER_27, gpio_base + GPIO_CLR_0_OFFSET);
}
EXPORT_SYMBOL(gpio_set_low);

void gpio_cleanup(void) {
    gpio_set_low();  // turn off led on cleanup
    if (gpio_base)
        iounmap(gpio_base);
}
EXPORT_SYMBOL(gpio_cleanup);