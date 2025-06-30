obj-m := led_cdev.o gpio_ctrl.o

KDIR := /home/hungle/YOCTO/poky/build/tmp/work/raspberrypi0_wifi-poky-linux-gnueabi/linux-raspberrypi/6.6.63+git/linux-raspberrypi-6.6.63+git
ARCH := arm
CROSS_COMPILE := arm-linux-gnueabihf-

all:
	make -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
	$(CROSS_COMPILE)gcc user_app.c -o led_app

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f led_app