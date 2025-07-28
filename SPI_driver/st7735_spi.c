#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#include "st7735_cmds.h"
#include "image_data.h"

struct st7735 {
    struct spi_device *spi;   // connect to SPI device
    struct gpio_desc *dc;     // data/command control pin
    struct gpio_desc *reset;  // reset pin
    struct gpio_desc *led;    // backlight LED pin (optional)
    struct task_struct *anim_thread;  // thread for animation continuously
};


// 0 -> command
static void st7735_write_command(struct st7735 *lcd, u8 cmd)
{
    gpiod_set_value_cansleep(lcd->dc, 0);
    spi_write(lcd->spi, &cmd, 1);
}
// 1 -> data
static void st7735_write_data(struct st7735 *lcd, u8 data)
{
    gpiod_set_value_cansleep(lcd->dc, 1);
    spi_write(lcd->spi, &data, 1);
}


// Set the address window for drawing
static void st7735_set_address_window(struct st7735 *lcd, u16 x0, u16 y0, u16 x1, u16 y1)
{
    st7735_write_command(lcd, ST7735_CASET);
    st7735_write_data(lcd, 0x00);
    st7735_write_data(lcd, x0);
    st7735_write_data(lcd, 0x00);
    st7735_write_data(lcd, x1);

    st7735_write_command(lcd, ST7735_RASET);
    st7735_write_data(lcd, 0x00);
    st7735_write_data(lcd, y0);
    st7735_write_data(lcd, 0x00);
    st7735_write_data(lcd, y1);

    st7735_write_command(lcd, ST7735_RAMWR);
}


// Initialize the display
static void st7735_init_display(struct st7735 *lcd)
{
    gpiod_set_value_cansleep(lcd->reset, 0);
    msleep(50);
    gpiod_set_value_cansleep(lcd->reset, 1);
    msleep(50);

    st7735_write_command(lcd, ST7735_SWRESET);  // Software reset
    msleep(150);

    st7735_write_command(lcd, ST7735_SLPOUT);  // Sleep out
    msleep(500);

    st7735_write_command(lcd, ST7735_COLMOD);  // Interface pixel format
    st7735_write_data(lcd, 0x05); // 16-bit color
    msleep(10);

    st7735_write_command(lcd, ST7735_DISPON);  // Display on
    msleep(100);
}


static int draw_animation_thread(void *data)
{
    struct st7735 *lcd = data;
    // Run continuously
    while (!kthread_should_stop()) {
        for (int i = 0; i < FRAME_COUNT; i++) {
            st7735_set_address_window(lcd, 0, 0, FRAME_WIDTH - 1, FRAME_HEIGHT - 1);  // Full screen
            for (int j = 0; j < FRAME_WIDTH * FRAME_HEIGHT; j++) {
                st7735_write_data(lcd, animation_frames[i][j] >> 8);
                st7735_write_data(lcd, animation_frames[i][j] & 0xFF);
            }
            msleep(120);  // Delay between frames to create animation effect
        }
    }
    return 0;
}


static int st7735_probe(struct spi_device *spi)
{
    struct st7735 *lcd;

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
    if (!lcd)
        return -ENOMEM;

    lcd->spi = spi;
    spi_set_drvdata(spi, lcd);

    // Get GPIOs from device tree (dc, reset, led)
    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);
    if (IS_ERR(lcd->dc))
        return dev_err_probe(&spi->dev, PTR_ERR(lcd->dc), "Failed to get dc-gpio\n");

    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_LOW);
    if (IS_ERR(lcd->reset))
        return dev_err_probe(&spi->dev, PTR_ERR(lcd->reset), "Failed to get reset-gpio\n");

    lcd->led = devm_gpiod_get_optional(&spi->dev, "led", GPIOD_OUT_HIGH);
    if (IS_ERR(lcd->led))
        return dev_err_probe(&spi->dev, PTR_ERR(lcd->led), "Failed to get led-gpio\n");

    // Optionally setup SPI if not configured in device tree
    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi_setup(spi);

    st7735_init_display(lcd);  // open the display lcd

    // Create a kernel thread for animation
    lcd->anim_thread = kthread_run(draw_animation_thread, lcd, "st7735_anim");
    if (IS_ERR(lcd->anim_thread)) {
        int err = PTR_ERR(lcd->anim_thread);
        dev_err(&spi->dev, "Failed to create animation thread: %d\n", err);
        return err;
    }

    dev_info(&spi->dev, "ST7735 display initialized and animation started\n");
    return 0;
}


static void st7735_remove(struct spi_device *spi)
{
    struct st7735 *lcd = spi_get_drvdata(spi);
    if (lcd->anim_thread)
        kthread_stop(lcd->anim_thread);   // Stop the animation thread
}

// Device tree match table
static const struct of_device_id st7735_of_match[] = {
    { .compatible = "hungle,st7735-spi" },
    {},
};
MODULE_DEVICE_TABLE(of, st7735_of_match);

static const struct spi_device_id st7735_id[] = {
    { "st7735-spi", 0 },  
    { "st7735", 0 },           
    {},
};
MODULE_DEVICE_TABLE(spi, st7735_id);

// SPI driver structure
static struct spi_driver st7735_driver = {
    .driver = {
        .name = "st7735-spi",
        .of_match_table = st7735_of_match,
    },
    .probe = st7735_probe,
    .remove = st7735_remove,
    .id_table = st7735_id,
};

// Register the SPI driver with the kernel
module_spi_driver(st7735_driver); 

MODULE_DESCRIPTION("ST7735 SPI Display Driver with Animation");
MODULE_AUTHOR("hungle le771485@gmail.com");
MODULE_LICENSE("GPL");
