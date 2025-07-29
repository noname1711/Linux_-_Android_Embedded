# What is I2C driver ?

It is Inter-Integrated Circuit.

The I2C protocol is commonly used for short-range communication via two signal lines (Two Wired Interface – TWI):
+ SDA (Serial Data): Line for exchanging data between signals
+ SCL (Serial Clock): Line for transmitting clock signals

Can be called by other names such as IIC, TWI, TWSI.

```text
          +------------------- I2C Bus -------------------+
          |                                               |
      +--------+         SDA (Data Line)        +----------+
      |        | <----------------------------> |          |
      | Master |                                |  Slave   |
      | (e.g., |         SCL (Clock Line)       | (Device  |
      |  CPU)  | <----------------------------> | Adapter) |
      +--------+                                +----------+
```
It is bus-master control communication (controlled by master) the device itself cannot receive or send but must go through the master.

Compare between Adapter and Client:
```text
Adapter (Master / Controller)       | Client (Slave / Device)
----------------------------------- | -----------------------------------
- Initiates all transactions        | - Responds to transactions
- Usually only one master (multi-   | - Multiple clients per bus
  master possible)                  |
- Has no address                    | - Has 7-bit hardware address  
                                    |   (10-bit extension possible)
- Controls the I2C bus (SCL & SDA)  | - Operates under master's clock
```

# Communication in I2C

+ Step 1: Start Condition

    The Master (Adapter) initiates communication by pulling the SDA line from HIGH → LOW while SCL is still HIGH.

    This signals all Slave (Client) devices that a transmission is about to begin.

+ Step 2: Address + R/W Bit

    The Master sends a 7-bit (or 10-bit) address followed by a 1-bit direction flag (0 = Write, 1 = Read).

    This is broadcast to all Slaves on the bus.

+ Step 3: Acknowledge (ACK)

    Each Slave compares the received address with its own.

    If matched, that Slave pulls SDA LOW for one clock cycle to send an ACK.

    Other Slaves stay idle.

+ Step 4: Data Transfer

    The Master and the addressed Slave exchange 8-bit data frames.

    Data is transferred bit by bit on SDA, synchronized with SCL pulses.

+ Step 5: ACK after Each Byte

    After each 8-bit frame, the receiver (Master or Slave) pulls SDA LOW for 1 clock cycle (ACK) to confirm successful receipt.

+ Step 6: Stop Condition

    The Master ends communication by releasing SCL HIGH, then switching SDA from LOW → HIGH.

    This signals the bus is now free for other transactions.

# The SMBus protocol 

+ Designed for chip communication on PC motherboards
+ Mostly a subset of I2C
+ Defines several commands
    + Register write: S addr + W A reg A data P
    + Register read: A addr + W A reg A RS addr + R A data N A P
+ Ofter I2C and SMBus clients can be mixed on the same bus

# I2C in Linux driver model 

```text
┌───────────────────┐        Probe / Remove        ┌──────────────────────┐        ┌───────────────────────┐
│  struct i2c_client │ ─────────────────────────▶ │    I2C Subsystem      │ ─────▶ │    struct i2c_adapter  │
│ (I2C Device Driver)│ ◀──────────────────────────│   (Core I2C Layer)    │ ◀───── │   (I2C Adapter Driver) │
└─────────┬─────────┘                             └──────────┬───────────┘        └───────────┬───────────┘
          │        Data Transfer: i2c_transfer()             │  .master_xfer()                  │ HW Access:
          ▼                                                  ▼                                 ▼ readl()/writel()
┌───────────────────┐                               ┌──────────────────────┐        ┌───────────────────────┐
│   GPIO Subsystem   │  <─────── Bit-banging ─────── │      I2C Bus         │ <───── │   Hardware Controller │
│ (GPIO Chip Driver) │                               │  (Protocol Signals)  │        │   (Physical Adapter)  │
└───────────────────┘                               └──────────────────────┘        └───────────────────────┘
          ▲                                                                                        
          │ IRQ (Interrupts propagate upward)                                                      
          └───────────────────────────────────────────────────────────────────────────────────────

Flow:
  1️⃣ probe() → Device driver registers with I2C subsystem  
  2️⃣ i2c_transfer() → Request sent to I2C core  
  3️⃣ master_xfer() → Adapter driver executes transaction  
  4️⃣ readl()/writel() → Low-level hardware I/O  
  5️⃣ IRQ → Interrupt notification travels up to device driver  
  6️⃣ remove() → Driver cleanup and resource release  
```

I2C Device includes both adapters and clients:
```bash
ls -l /sys/bus/i2c/devices
```
Some number such as "0-0039" is bus, "40012000" is the address in the register of the I2C controller

And something belike "i2c-0", "i2c-1" is master. We need change status "disable" to "okay" if i2c0, i2c1 turn off in node of device tree.

# LCD1602 I2C Driver print Hello world!

Wiring diagram
```text
+---------------------------+            +-----------------------------+
| Raspberry Pi Zero W      |            | LCD1602 Display (I2C)       |
|---------------------------|            |-----------------------------|
| GPIO 2  (I2C1_SDA) -------- I2C SDA -->| SDA                         |
| GPIO 3  (I2C1_SCL) -------- I2C SCL -->| SCL                         |
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

Insert some tools to local.conf:
```bash
RPI_EXTRA_CONFIG += "dtoverlay=my-i2c"
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
