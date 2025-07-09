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
Now flash agian
```bash
cd ~/YOCTO/poky/build/tmp/deploy/images/raspberrypi0-wifi/
sudo dd if=core-image-base-raspberrypi0-wifi.rootfs.wic of=/dev/mmcblk0 bs=4M status=progress && sync
```
Connect microSD to raspberry pi rezo w

Go to this program, command
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