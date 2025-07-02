Firstly, in this folder:
```bash
source /opt/poky/5.2/environment-setup-arm1176jzfshf-vfp-poky-linux-gnueabi
```
Check your GPIO (GPIO = gpio base + offset, in my example, it is 17 + 512)
```bash
grep 17 /sys/kernel/debug/gpio
```
Build program:
```bash
make
```
Connect the microSD before next command:
```bash
sudo mkdir /media/hungle/root/led_integer_based
sudo cp led_ctl.ko app_ledctl /media/hungle/root/led_integer_based/
```
Open the microSD:
```bash
lsblk
```
We will see:
```text
mmcblk0     179:0    0  59.5G  0 disk 
├─mmcblk0p1 179:1    0   130M  0 part /media/hungle/boot
└─mmcblk0p2 179:2    0 405.6M  0 part /media/hungle/root
```
Now, we can see folder led_interger_based by command:
```bash
ls /media/hungle/root/
```
Last action, command:
```bash
sync
sudo umount /media/hungle/boot
sudo umount /media/hungle/root
```
Flash to raspberry pi rezo W, open minicom by uart:
```bash
sudo minicom -D /dev/ttyUSB0 -b 115200
```
Login by root and go to our folder
```bash
cd /led_integer_based/
```
Now, we can
```bash
insmod led_ctl.ko
chmod +x app_ledctl
```
And this is the result, on/off led from GPIO17
```bash
/app_ledctl on
```
or
```bash
./app_ledctl off
```

