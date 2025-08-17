# BringUp and Porting 

```text
+------------------------+-------------------------------------------+-------------------------------------------+
| **Aspect**             | **Bring up**                              | **Porting**                               |
+------------------------+-------------------------------------------+-------------------------------------------+
| **Definition**         | Initial hardware/software setup to boot   | Adapting software to run on a new platform|
|                        | a system (e.g., board, SoC, firmware).    | (OS, architecture, or environment).      |
+------------------------+-------------------------------------------+-------------------------------------------+
| **Key Goals**          | - First-stage bootloader (e.g., U-Boot)   | - Maintain functionality on new target.   |
|                        | - Kernel booting (e.g., Linux, Zephyr).   | - Resolve dependencies (libs, APIs).      |
|                        | - Basic peripheral drivers (e.g., UART).  | - Optimize for performance/resource limits|
+------------------------+-------------------------------------------+-------------------------------------------+
| **Real-World Challenges** |                                         |                                           |
| **Hardware Issues**    | - Incorrect clock/power settings.         | - Missing hardware features (e.g., GPU).  |
|                        | - Dead boards due to PCB defects.         | - Different endianness (ARM vs. x86).     |
| **Software Issues**    | - Bootloader crashes (e.g., DRAM init).   | - Incompatible syscalls (Linux ↔ Windows).|
|                        | - Kernel panics (missing device tree).    | - Library version conflicts (glibc vs. musl).|
| **Debugging**          | - Requires JTAG/SWD probes, oscilloscope. | - Cross-platform emulators (QEMU, WINE).  |
|                        | - Serial console logs (often corrupted).  | - Conditional compilation (#ifdef hell).  |
+------------------------+-------------------------------------------+-------------------------------------------+
| **Tools Used**         | - OpenOCD, JTAG debuggers.                | - Cross-compilers (gcc-arm, clang).       |
|                        | - Logic analyzers, multimeters.           | - Compatibility layers (WSL, Rosetta 2).  |
|                        | - U-Boot, TF-A, Coreboot.                 | - Static analyzers (e.g., `checksec`).    |
+------------------------+-------------------------------------------+-------------------------------------------+
| **Time/Cost**          | - High: Weeks/months for custom HW.       | - Moderate: Days/weeks for well-documented|
|                        | - Costs scale with board complexity.      |   code; months for legacy systems.        |
+------------------------+-------------------------------------------+-------------------------------------------+
| **Real-World Examples**| - Booting a Raspberry Pi CM4 on custom PCB| - Porting Android apps to iOS (Flutter).  |
|                        | - Debugging a bricked ESP32 (flash issues)| - Running Linux binaries on FreeBSD.      |
+------------------------+-------------------------------------------+-------------------------------------------+
| **Mitigation Strategies** |                                       |                                           |
| **Prevention**         | - Validate reference designs.             | - Use portable code (POSIX, HAL layers).  |
|                        | - Test early with development kits.       | - Isolate platform-specific code.         |
| **Debugging**          | - Capture early boot logs (RAM console).  | - Automated testing (CI/CD on new target).|
|                        | - Use known-good binaries (golden images).| - Fuzz testing (e.g., AFL, libFuzzer).    |
+------------------------+-------------------------------------------+-------------------------------------------+
```
In short, it is as follows:
+ BringUp: is the process of bringing a new board from zero to a basic working state: Power up -> Bootloader -> Kernel -> Userspace.
+ Porting: is to convert an OS, driver or software from hardware platform A to run on hardware platform B.

-> Bringup will take more time, so in reality, we usually prioritize speed and efficiency, so we often use Porting. 

However, due to copyright issues, the cost of Porting is also higher than Bringup, so sometimes we also consider using Bringup.


# For LCD 2.4inch 240x320 ILI9341 Resistive Touch SPI Interface

Connect Raspberry PI Zero W to ILI9341 Display Module (display and touching screen) and UART (for debugging):
```text
             +---------------------------+            +-----------------------------+
             | Raspberry Pi Zero W       |            |   ILI9341 + XPT2046 Module  |
             |---------------------------|            |-----------------------------|
   POWER 🔌  | PIN 6  (GND)  ------------ GND   ----->| GND                         |
             | PIN 4  (5V)  ------------ VCC   ----->| VCC                         |
             |---------------------------|            |-----------------------------|
   LCD ⚙️    | PIN 11 (GPIO17) --------- GPIO  ----->| LED   (Backlight)           |
             | PIN 13 (GPIO27) --------- GPIO  ----->| DC    (Data/Command)        |
             | PIN 24 (GPIO8)  --------- SPI CS ---->| CS_LCD (ILI9341)            |
             | PIN 15 (GPIO22) --------- GPIO  ----->| RESET                       |
             |---------------------------|            |-----------------------------|
   DATA 📡   | PIN 23 (GPIO11) --------- SPI CLK --->| SCK                         |
             | PIN 19 (GPIO10) --------- SPI MOSI -->| SDI (MOSI)                  |
             | PIN 21 (GPIO9)  <-------- SPI MISO ---| SDO (MISO)                  |
             |---------------------------|            |-----------------------------|
   TOUCH 🖐  | PIN 26 (GPIO7)  --------- SPI CS ---->| T_CS  (XPT2046)             |
             | PIN 22 (GPIO25) <-------- GPIO  ------| T_IRQ (Interrupt)           |
             | PIN 23 (GPIO11) --------- SPI CLK --->| T_CLK                       |
             | PIN 19 (GPIO10) --------- SPI MOSI -->| T_DIN                       |
             | PIN 21 (GPIO9)  <-------- SPI MISO ---| T_DO                        |
             +---------------------------+            +-----------------------------+


                                    ⬇️ UART DEBUG CONNECTION ⬇️

             +---------------------------+            +-----------------------------+
             | Raspberry Pi Zero W       |            | UART to USB Adapter (PC)    |
             |---------------------------|            |-----------------------------|
             | PIN 8  (GPIO14, TX) ------ UART TX --->| RXD                         |
             | PIN 10 (GPIO15, RX) ------ UART RX <---| TXD                         |
             | PIN 6  (GND)        ------ GND    ---->| GND                         |
             +---------------------------+            +-----------------------------+
```


Check if driver for ILI9341 exists in yocto:
```bash
bitbake -c devshell virtual/kernel
```
and check 
```bash
find drivers -name "*ili9341*"
```

Luckily, I have 3 drivers:
```bash
drivers/staging/fbtft/fb_ili9341.c
drivers/gpu/drm/tiny/ili9341.c
drivers/gpu/drm/panel/panel-ilitek-ili9341.c
```
+ fb_ili9341.c: Frame buffer 
+ ili9341.c: TinyDRM
+ panel-ilitek-ili9341.c: DRM panel driver

Now, we will use framebuffer, it is the best way to use Qt for ILI9341 screen (quickly building).

Enable framebuffer:
```bash
bitbake -c menuconfig virtual/kernel
```
And:
```bash
Device Drivers  --->
   Staging drivers  --->
      <M> Support for small TFT LCD display modules (FB_TFT)
         <M>   FB driver for the ILI9341 LCD Controller
```
Firstly, enter SPACE to change Support for small TFT LCD display modules from <M> to <*>

FB driver for the ILI9341 LCD Controller still display <M> (build module .ko), we will enter SPACE to change to <*> (built-in). 

Now, we should check Support for frame buffer device drivers again, if <M> we must be change to <*> (but usually it will be <*>)
```bash
Device Drivers --->
  Graphic support --->
    Frame buffer Devices  --->
      <*> Support for frame buffer device drivers  --->
```

Save config and rebuild:
```bash
bitbake -c savedefconfig virtual/kernel
bitbake virtual/kernel
```

Insert Qt to Yocto Image

+ Firstly, clone meta-qt6
```bash
cd YOCTO/poky
git clone -b 6.9 https://code.qt.io/yocto/meta-qt6.git
```
Insert layer "meta-qt6" to bblayers.conf
```bash
bitbake-layers add-layer ../meta-qt6
```

+ Create our custom metalayer, structure belike: 
```bash
├── conf
│   └── layer.conf
├── COPYING.MIT
├── README
├── recipes-example
│   └── example
│       └── example_0.1.bb
└── recipes-qt
    └── qt-ILI9341
        ├── files
        └── qt-ILI9341_1.0.bb
```
In files, it has src of qt app, and code file .bb such as:
```bash
SUMMARY = "Qt6 ILI9341 doreamon application"
DESCRIPTION = "GUI app built with Qt6 using ILI9341 display"
LICENSE = "CLOSED"

SRC_URI = " \
    path to src
"

S = "${WORKDIR}"

DEPENDS += "qtbase qtdeclarative qttools"

inherit qt6-qmake

do_install() {
    install -d ${D}${bindir}
    install -m 0755 qt_ILI9341 ${D}${bindir}/
}
```

+ Open local.config and install my Qt app:
```conf
IMAGE_INSTALL:append = " qt-ILI9341"
```

And rebuild image
```bash
bitbake core-image-sato
```

Now, we will compile overlay
```bash
dtc -@ -I dts -O dtb -o ili9341.dtbo ili9341-overlay.dts
```