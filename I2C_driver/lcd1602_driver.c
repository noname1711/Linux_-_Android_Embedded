#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/delay.h>

#define LCD_ADDR 0x27  
#define LCD_BACKLIGHT 0x08
#define ENABLE 0x04
#define LCD_LINE1 0x80

static struct i2c_client *lcd_client;

static void lcd_send_nibble(u8 nibble)
{
    u8 data[4];
    data[0] = nibble | LCD_BACKLIGHT | ENABLE;
    data[1] = nibble | LCD_BACKLIGHT;
    i2c_master_send(lcd_client, data, 2);
    udelay(50);
}

static void lcd_send_byte(u8 byte, u8 mode)
{
    u8 high = (byte & 0xF0) | mode;
    u8 low  = ((byte << 4) & 0xF0) | mode;
    lcd_send_nibble(high);
    lcd_send_nibble(low);
}

static void lcd_write_cmd(u8 cmd)
{
    lcd_send_byte(cmd, 0x00); // RS = 0
    mdelay(2);
}

static void lcd_write_data(u8 data)
{
    lcd_send_byte(data, 0x01); // RS = 1
    mdelay(2);
}

static void lcd_init(void)
{
    mdelay(50);
    lcd_send_nibble(0x30);
    mdelay(5);
    lcd_send_nibble(0x30);
    udelay(150);
    lcd_send_nibble(0x30);
    lcd_send_nibble(0x20); 

    lcd_write_cmd(0x28); 
    lcd_write_cmd(0x0C); 
    lcd_write_cmd(0x06); 
    lcd_write_cmd(0x01); 
    mdelay(5);
}

static void lcd_print(const char *str)
{
    lcd_write_cmd(LCD_LINE1);
    while (*str)
        lcd_write_data(*str++);
}

// match "compatible"
static int lcd1602_probe(struct i2c_client *client)
{
    pr_info("lcd1602: probe called!\n");

    lcd_client = client;

    lcd_init();
    lcd_print("Hello world!");

    return 0;
}

static void lcd1602_remove(struct i2c_client *client)
{
    pr_info("lcd1602: remove called!\n");

    lcd_write_cmd(0x01); // Clear screen
}

static const struct of_device_id lcd1602_dt_ids[] = {
    { .compatible = "lcd1602" },
    { }
};
MODULE_DEVICE_TABLE(of, lcd1602_dt_ids);

static const struct i2c_device_id lcd1602_id[] = {
    { "lcd1602", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, lcd1602_id);

static struct i2c_driver lcd1602_driver = {
    .driver = {
        .name = "lcd1602",
        .of_match_table = lcd1602_dt_ids,
    },
    .probe  = lcd1602_probe,
    .remove = lcd1602_remove,
};

module_i2c_driver(lcd1602_driver);

MODULE_AUTHOR("HungLe le771485@gmail.com");
MODULE_DESCRIPTION("LCD1602 I2C Driver - Hello world!");
MODULE_LICENSE("GPL");
