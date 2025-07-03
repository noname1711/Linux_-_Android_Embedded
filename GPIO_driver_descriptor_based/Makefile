obj-m += led_driver.o  

KDIR := /home/hungle/YOCTO/poky/build/tmp/work/raspberrypi0_wifi-poky-linux-gnueabi/linux-raspberrypi/6.6.63+git/linux-raspberrypi-6.6.63+git

PWD := $(shell pwd)
APP := led_app  

all: modules $(APP)

modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(APP): led_app.c
	$(CC) -o $@ $<

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	$(RM) -f $(APP)
