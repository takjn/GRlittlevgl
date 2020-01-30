# LittlevGL for Gadget Renesas
LittlevGL demo for GR-LYCHEE
![image](https://raw.githubusercontent.com/takjn/GRlittlevgl/master/image.jpg)

## How to use
- `mbed import https://github.com/takjn/GRlittlevgl`
- `mbed compile` and copy the binary file to your board.
- Connect a LCD and a USB mouse.
- Reset the board.

## Note
### Change display resolution
The default resolution is 480 x 272.
If you want to use another display, please change `LV_HOR_RES_MAX` and `LV_VER_RES_MAX` in `lv_conf.h`.

### USB mouse
If the mouse doesn't work well, you may need a hack in `mbed-gr-libs/USBHost_custom/USBHostHID/USBHostMouse.cpp`.

```
    // if ((dev->getVid() == 0x046d) && (dev->getPid() == 0xc52b)) {
    if (1) {
```

### mbed version
```
$ mbed --version
1.10.2
$ arm-none-eabi-g++ --version
arm-none-eabi-g++ (15:6.3.1+svn253039-1build1) 6.3.1 20170620
```

## Reference
- [GR-LYCHEE](https://www.renesas.com/us/ja/products/gadget-renesas/boards/gr-lychee.html)
- [LittlevGL](https://littlevgl.com/)
- [Examples for Littlev Graphics Library](https://github.com/littlevgl/lv_examples)
