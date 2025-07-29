#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/delay.h>   // For mdelay and udelay

#define LCD_ADDR 0x27  // I2C address of the LCD1602
#define LCD_BACKLIGHT 0x08  // Backlight control bit
#define ENABLE 0x04  // Bit to generate EN (Enable) pulse for LCD to receive data.
#define LCD_LINE1 0x80  // Command to set cursor to the first line

static struct i2c_client *lcd_client;


static void lcd_send_nibble(u8 nibble)
{
    u8 data[4];  // LCD 1602 communicates in 4-bit mode, sending half bytes (nibbles) at a time
    data[0] = nibble | LCD_BACKLIGHT | ENABLE;   // Generate up pulse (EN = 1)
    data[1] = nibble | LCD_BACKLIGHT;  // Generate down pulse (EN = 0)
    i2c_master_send(lcd_client, data, 2);   // Send 2 bytes to I2C chip
    udelay(50);  // Wait for the LCD to process the command
}


// LCD receives 8-bit data, but because it runs in 4-bit mode, it splits it into two nibbles.
static void lcd_send_byte(u8 byte, u8 mode)
{
    u8 high = (byte & 0xF0) | mode;
    u8 low  = ((byte << 4) & 0xF0) | mode;
    // mode = 0x00: send RS = 0 (Command)
    // mode = 0x01: send RS = 1 (Data)
    lcd_send_nibble(high);    // Send 4 bits (high nibble)
    lcd_send_nibble(low);     // Send 4 bits (low nibble)
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
    // Initialize the LCD in 4-bit mode, but datasheet requires send 3 times 0x30 first
    lcd_send_nibble(0x30);   // Function set: 8-bit mode, first time
    mdelay(5);
    lcd_send_nibble(0x30);   // Function set: 8-bit mode, second time
    udelay(150);
    lcd_send_nibble(0x30);   // Function set: 8-bit mode, third time
    lcd_send_nibble(0x20);   // Function set: LCD covert to 4-bit mode
    // After this, all commands will send 2 nibbles (4 bits): high and low

    lcd_write_cmd(0x28); 
    // 0x28 = Function set: 4-bit mode (DL=0), 2 lines (N=1), font 5x8 dots (F=0)
    lcd_write_cmd(0x0C); 
    // 0x0C = Display ON, Cursor OFF, Blink OFF (D=1, C=0, B=0)
    // open backlight, but no flashing cursor display.
    lcd_write_cmd(0x06); 
    // 0x06 = Entry mode set: Increment cursor, no shift (I/D=1, S=0)
    lcd_write_cmd(0x01); 
    // 0x01 = Clear display, cursor return home (first line)
    mdelay(5);
}


static void lcd_print(const char *str)
{
    // Place the pointer on line 1 and write
    lcd_write_cmd(LCD_LINE1);
    while (*str)
        lcd_write_data(*str++);
}


// match "compatible"
static int lcd1602_probe(struct i2c_client *client)
{
    pr_info("lcd1602: probe called!\n");
    lcd_client = client;  // Store the i2c_client for later use
    lcd_init();           // Initialize the LCD
    lcd_print("Hello world!");
    return 0;
}

static void lcd1602_remove(struct i2c_client *client)
{
    pr_info("lcd1602: remove called!\n");
    lcd_write_cmd(0x01); // Clear screen before remove driver
}


// Device tree support (open firmware)
static const struct of_device_id lcd1602_dt_ids[] = {
    { .compatible = "lcd1602" },
    { }
};
MODULE_DEVICE_TABLE(of, lcd1602_dt_ids);

// Ìf the device is not found in the device tree, it will be matched by the I2C ID table
static const struct i2c_device_id lcd1602_id[] = {
    { "lcd1602", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, lcd1602_id);

// Define the I2C driver structure
static struct i2c_driver lcd1602_driver = {
    .driver = {
        .name = "lcd1602",
        .of_match_table = lcd1602_dt_ids,
    },
    .probe  = lcd1602_probe,
    .remove = lcd1602_remove,
};

module_i2c_driver(lcd1602_driver);  // Register the driver with the I2C subsystem

// Metadata for driver
MODULE_AUTHOR("HungLe le771485@gmail.com");
MODULE_DESCRIPTION("LCD1602 I2C Driver - Hello world!");
MODULE_LICENSE("GPL");
