Wiring diagram
```text
+---------------------------+           +----------------------------+
| Raspberry Pi Zero W      |           | LED + Resistor             |
|---------------------------|           |----------------------------|
| GPIO 27             ------- GPIO --->| Anode (+)                  |
| GND                 ------- GND  --->| Cathode (-) via 330Ω       |
+---------------------------+           +----------------------------+

                    ⬇️ UART DEBUG CONNECTION ⬇️

+---------------------------+           +----------------------------+
| Raspberry Pi Zero W      |           | UART to USB Adapter (PC)   |
|---------------------------|           |----------------------------|
| GPIO 14 (UART TX) -------- UART TX -->| RXD                        |
| GPIO 15 (UART RX) -------- UART RX <--| TXD                        |
| GND                 ------- GND ----->| GND                        |
+---------------------------+           +----------------------------+
```
In this folder
```bash
source /opt/poky/5.2/environment-setup-arm1176jzfshf-vfp-poky-linux-gnueabi
```
And build program 
```bash
make
```
Make folder in microSD and insert file to this
```bash
sudo mkdir -p /media/hungle/root/root/led_gpio27/
sudo cp led_driver.ko led_app /media/hungle/root/root/led_gpio27/
```


Go to .dts
```bash
cd /home/hungle/YOCTO/poky/build/tmp/work/raspberrypi0_wifi-poky-linux-gnueabi/linux-raspberrypi/6.6.63+git/linux-raspberrypi-6.6.63+git
```
Open file
```bash
code arch/arm/boot/dts/bcm2835-rpi-zero-w.dts
```
And insert node here
```dts
    led_test: led_test {
        compatible = "gpio-descriptor-based";
        led27-gpios = <&gpio 27 0>;
        status = "okay";
    };
```
Build kernel again 
