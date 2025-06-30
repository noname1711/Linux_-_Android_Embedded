#ifndef LED_GPIO_CTRL_H
#define LED_GPIO_CTRL_H

#include <linux/io.h> 

// Raspberry Pi Zero W
#define BCM2835_GPIO_BASE_ADDR  0x20200000  
#define GPIO_FSEL_OFFSET        0x00
#define GPIO_SET_0_OFFSET       0x1C
#define GPIO_CLR_0_OFFSET       0x28
#define GPIO_NUMBER_27          27

extern void gpio_setup(void);
extern void gpio_set_high(void);
extern void gpio_set_low(void);
extern void gpio_cleanup(void);

#endif
