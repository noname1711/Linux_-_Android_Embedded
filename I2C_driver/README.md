Insert some tools to local.conf:
```bash
IMAGE_INSTALL:append = " i2c-tools lcd1602-driver"
PREFERRED_PROVIDER_virtual/kernel = "linux-raspberrypi"
```
Build again
```bash
bitbake virtual/kernel -c devshell
```
Insert node to device tree
```bash
cd ~/YOCTO/poky/build/tmp/work-shared/raspberrypi0-wifi/kernel-source/arch/arm/boot/dts 
cat broadcom/bcm2835-rpi-zero-w.dts
```
This node is 
```dts
&i2c1 {
    status = "okay";

    lcd1602@27 {
        compatible = "hungle,lcd1602-i2c";
        reg = <0x27>;
        display-height-chars = <2>;
        display-width-chars = <16>;
    };
};
```
Rebuild again:
```bash
bitbake virtual/kernel -c compile -f
bitbake virtual/kernel -c deploy
```
Now we have file Device Tree Binary, just copy to microSD
```bash
cp tmp/deploy/images/raspberrypi0-wifi/bcm2708-rpi-zero-w.dtb /media/hungle/boot/
```

Command in this folder
```bash
source /opt/poky/5.2/environment-setup-arm1176jzfshf-vfp-poky-linux-gnueabi
make
```
Now we have lcd1602_driver.ko, copy this to microSD
```bash
sudo cp lcd1602_driver.ko /media/hungle/root/home/root/
```
Now, we can use microSD for raspberry pi rezo w after
```bash
sync
sudo umount /media/hungle/boot
sudo umount /media/hungle/root
```
Open minicom by uart, command 
```bash
sudo minicom -D /dev/ttyUSB0 -b 115200
```
Login by root and insmod lcd1602_driver.ko
```bash
insmod lcd1602_driver.ko
```
If the program don't run (lcd don't print "Hello world"), assign device to driver, lcd1602 using address is 0x27, and bus is i2c-1
```bash
echo lcd1602 0x27 > /sys/bus/i2c/devices/i2c-1/new_device
```
If you want to remove
```bash
rmmod lcd1602_driver
```
After all, we should be clean all
```bash
make all
```
