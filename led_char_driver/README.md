Create a character device file and an app in userspace. If the app writes "on" then the kernel will print the string "turn led on", if it writes "off" then the kernel will print the string "turn led off"

command:
```bash
make
```
now:
```bash
sudo insmod led_char.ko
```
Open app with this command:
```bash
gcc -o led_app led_app.c
sudo ./led_app
```
Open new terminal to see status:
```bash
dmesg -w
```
After all, delete this:
```bash
make clean
```
