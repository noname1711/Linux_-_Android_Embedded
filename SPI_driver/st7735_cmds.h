#ifndef _ST7735_CMDS_H_
#define _ST7735_CMDS_H_

#define ST7735_NOP     0x00   // No operation
#define ST7735_SWRESET 0x01   // Software reset
#define ST7735_SLPOUT  0x11   // Sleep out
#define ST7735_DISPON  0x29   // Display on
#define ST7735_CASET   0x2A   // Column address set
#define ST7735_RASET   0x2B   // Row address set
#define ST7735_RAMWR   0x2C   // Write to RAM of display screen
#define ST7735_MADCTL  0x36   // Memory data access control
#define ST7735_COLMOD  0x3A   // Interface pixel format

#endif 
