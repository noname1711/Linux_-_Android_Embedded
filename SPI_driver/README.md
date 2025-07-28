# What is SPI ?

It is Serial Peripheral Interface: 
+ Developed by Motorola
+ Mechanism: master - slave bus
+ Have 4 wires bus
+ No max clock speed
+ High communication speed

Used when:
+ Flash memory
+ ADCs 
+ Sensor
+ LCD controllers
+ Chromium embedded controller

# SPI structure

SPI signals:
+ MOSI: Master Output Slave Input    Ex: SIMO, SDI, DI, SDA
+ MISO: Master Input Slave Output    Ex: SOMI, SDO, DO, SDA
+ SCLK: Serial Clock (Master Output) Ex: SCK, CLK, SCL
+ SS: Slave Select (Master Output)
+ CSn, EN, ENB

SPI modes: 

Modes are composed of two clock characteristics:

+ CPOL - Clock polarity
    + 0 = Clock idle state low
    + 1 = Clock idle state high
+ CPHA - Clock phase
    + 0 = Data latched falling, output rising
    + 1 = Data latched rising, output falling


SPI Master and Slave:
```text
+-------------+                       +-------------+
|   MASTER    |                       |    SLAVE    |
|-------------|                       |-------------|
|   MOSI  ----+---------------------->|   MOSI       |
|   MISO  <---+-----------------------+--- MISO      |
|   SCLK  ----+---------------------->|   SCLK       |
|   SS    ----+---------------------->|   SS         |
|   GND  -----+-----------------------+--- GND       |
+-------------+                       +-------------+
```
MOSI: Master sends data → Slave receives

MISO: Slave sends data → Master receives

SCLK: Master generates clock pulse to synchronize data transmission

SS (Chip Select): Master pulls down to LOW to activate that slave


SPI can be more complicated:
+ Multiple SPI slaves: one chip select for each slave
+ Daisy chaining: 
    + Inputs to Outputs
    + Chip selects
+ Dual or Quad SPI (or more lanes):
    + Implemented in high speed SPI flash devices
    + Instead of one MISO, have N MISOs
    + N times bandwidth of traditional SPI
+ 3 wires SPI: combined MISO/MOSI signal operates in half duplex

SPI Master and Multiple Slaves:
```text
+-------------+                       +-------------+
|   MASTER    |                       |   SLAVE 1   |
|-------------|                       |-------------|
|   MOSI  ----+---------------------->|   MOSI       |
|   MISO  <---+-----------------------+--- MISO      |
|   SCLK  ----+---------------------->|   SCLK       |
|   SS0   ----+---------------------->|   SS         |
|                                         GND        |
|                                     +-------------+
|                                     +-------------+
|                                     |   SLAVE 2   |
|                                     |-------------|
|   MOSI  ----+---------------------->|   MOSI       |
|   MISO  <---+-----------------------+--- MISO      |
|   SCLK  ----+---------------------->|   SCLK       |
|   SS1   ----+---------------------->|   SS         |
|                                         GND        |
+-------------+                       +-------------+
```
MOSI, MISO, SCLK are shared between slaves.

Each slave needs a separate SS (SS0, SS1, …) for the Master to select the correct device.

When communicating with Slave 1 → Master pulls SS0 LOW and other SSs HIGH.

# Linux SPI drivers

Controller and protocol drivers only (so far)

Controller drivers support the SPI master controller: Driver hardware to control clock and chip selects, shift data bits on/off wire and configure basic SPI characteristics like clock frequency and mode

Protocol drivers support the SPI slave specific functionality 
+ Based on messages and transfers
+ Relies on controller driver to program SPI master hardware

# Linux SPI communication

Communication is broken up into transfers and messages:
+ Transfers:
    + Defines a single operation between master and slave
    + TX/RX buffer pointers
    + Optional chip select bahavior after operation
    + Optional delay after opration
+ Messages:
    + Atomic sequence of transfers
    + Fundermental argument to all SPI subsystem read/write APIs

# Userspace driver spidev

Allows direct communication with hardware without going through other drivers. 

SPI provides spidev for testing, however, some platforms will not run spidev if left on, so their status should be set to "disable".

Slave devices bound to the spidev driver yield:

    /sys/class/spidev/spidev[bus].[cs]

    /dev/spidev[bus].[cs]

Character device:
+ open/close
+ read/write are half duplex
+ ioctl():
    + SPI_IOC_MESSAGE: raw messages, full duplex and chip select control
    + SPI_IOC_[RD/WR]_*_set SPI parameters

# Exploring via usecases

+ I want to hook up a SPI device on my board that already has a protocol driver in the kernel.

+ I want to write a kernel protocol driver to control my SPI slave.

+ I want to write a kernel controller driver to drive my SPI master.

+ I want to write a userspace protocol driver to control my SPI slave.


# ST7735 SPI Display Driver with Animation

Wiring Diagram
```text
+---------------------------+            +-----------------------------+
| Raspberry Pi Zero W      |            | ST7735 Display Module       |
|---------------------------|            |-----------------------------|
| GPIO 10 (SPI0_MOSI) ------ SPI MOSI -->| DIN / SDA                   |
| GPIO 11 (SPI0_SCLK) ------ SPI CLK --->| CLK / SCK                   |
| GPIO 8  (SPI0_CE0)  ------ SPI CS  --->| CS                          |
| GPIO 25              ------ GPIO   --->| DC (Data/Command)           |
| GPIO 24              ------ GPIO   --->| RESET                       |
| GPIO 23              ------ GPIO   --->| LED / Backlight             |
| GND                 -------- GND ----->| GND                         |
| 3.3V or 5V          -------- VCC ----->| VCC                         |
+---------------------------+            +-----------------------------+

                    ⬇️ UART DEBUG CONNECTION ⬇️

+---------------------------+            +-----------------------------+
| Raspberry Pi Zero W      |            | UART to USB Adapter (PC)    |
|---------------------------|            |-----------------------------|
| GPIO 14 (UART TX) -------- UART TX --->| RXD                         |
| GPIO 15 (UART RX) -------- UART RX <---| TXD                         |
| GND                 ------- GND -----> | GND                         |
+---------------------------+            +-----------------------------+
```


Firstly, we should disabled spidev by menuconfig
```bash
cd YOCTO/poky/
source oe-init-build-env
bitbake -c menuconfig virtual/kernel
```
Go to Device Drivers -> SPI Support -> User mode SPI device support -> Click [SPACE]

Save and exit.

Now, we will rebuild
```bash
bitbake virtual/kernel -c savedefconfig
bitbake virtual/kernel
```

Flash image to microSD again, but we should remove the last image
```bash
sudo umount /dev/mmcblk0p1 2>/dev/null
sudo umount /dev/mmcblk0p2 2>/dev/null
sudo dd if=/dev/zero of=/dev/mmcblk0 bs=1M count=50 status=progress && sync
```
Now flash again
```bash
cd ~/YOCTO/poky/build/tmp/deploy/images/raspberrypi0-wifi/
sudo dd if=core-image-base-raspberrypi0-wifi.rootfs.wic of=/dev/mmcblk0 bs=4M status=progress && sync
```
Connect microSD to raspberry pi rezo w

Go to this program, command below to set up a cross-compilation environment for the Yocto SDK.
```bash
source /opt/poky/5.2/environment-setup-arm1176jzfshf-vfp-poky-linux-gnueabi
```
Now, we will build file .ko 
```bash
make
```
and file .dtbo
```bash
dtc -@ -I dts -O dtb -o my-st7735-overlay.dtbo my-st7735-overlay.dts
```
Now, connect microSD to host again, copy file .ko and file .dtbo to this
```bash
sudo cp my-st7735-overlay.dtbo /media/hungle/boot/overlays/
sudo cp st7735_spi.ko /media/hungle/root/home/root/
```
Edit file config.txt 
```bash
nano /media/hungle/boot/config.txt
```
and insert my-st7735-overlay
```text
dtoverlay=my-st7735-overlay
```
Sync microSD
```bash
sync
sudo umount /media/hungle/boot
sudo umount /media/hungle/root
```
After all, we should clean that
```bash
make clean
```

And open minicom from uart and login by root
```bash
sudo minicom -D /dev/ttyUSB0 -b 115200
```
Insmod file .ko and see the screen of your device
```bash
insmod /home/root/st7735_spi.ko
```