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