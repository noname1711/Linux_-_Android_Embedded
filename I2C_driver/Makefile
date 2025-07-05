obj-m += lcd1602_driver.o

# find KDIR: find tmp/work -type f -name ".config"
KDIR := /home/hungle/YOCTO/poky/build/tmp/work/raspberrypi0_wifi-poky-linux-gnueabi/linux-raspberrypi/6.6.63+git/linux-raspberrypi0_wifi-standard-build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
