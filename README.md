# LittlevGL for Gadget Renesas
LittlevGL demo for GR-LYCHEE

## How to use
- `mbed import https://github.com/takjn/GRlittlevgl`
- Run `mbed compile` and copy the binary file to your board.
- Connect a LCD and a USB mouse.
- Reset the board.

```
$ mbed --version
1.10.2
$ arm-none-eabi-g++ --version
arm-none-eabi-g++ (15:6.3.1+svn253039-1build1) 6.3.1 20170620
```

## Note
If the mouse doesn't work well, You may need a hack in `mbed-gr-libs/USBHost_custom/USBHostHID/USBHostMouse.cpp`.

```
    // if ((dev->getVid() == 0x046d) && (dev->getPid() == 0xc52b)) {
    if (1) {
```

## Reference
- [GR-LYCHEE](https://www.renesas.com/us/ja/products/gadget-renesas/boards/gr-lychee.html)
- [LittlevGL](https://littlevgl.com/)
- [Examples for Littlev Graphics Library](https://github.com/littlevgl/lv_examples)
