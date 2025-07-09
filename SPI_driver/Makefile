obj-m += st7735_spi.o

KDIR := /home/hungle/YOCTO/poky/build/tmp/work/raspberrypi0_wifi-poky-linux-gnueabi/linux-raspberrypi/6.6.63+git/linux-raspberrypi0_wifi-standard-build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
