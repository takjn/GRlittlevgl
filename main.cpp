#include "mbed.h"
#include "dcache-control.h"
#include "EasyAttach_CameraAndLCD.h"

#define FRAME_BUFFER_STRIDE    (((LCD_PIXEL_WIDTH * 2) + 31u) & ~31u)
#define FRAME_BUFFER_HEIGHT    (LCD_PIXEL_HEIGHT)

static uint8_t user_frame_buffer0[FRAME_BUFFER_STRIDE * FRAME_BUFFER_HEIGHT]__attribute((aligned(32)));

DisplayBase Display;

static void Start_LCD_Display(void) {
    DisplayBase::rect_t rect;

    // Initialize the background to black
    for (uint32_t i = 0; i < sizeof(user_frame_buffer0); i += 2) {
        user_frame_buffer0[i + 0] = 0x00;
        user_frame_buffer0[i + 1] = 0x00;
    }
    // Data cache clean
    dcache_clean(user_frame_buffer0, sizeof(user_frame_buffer0));

    rect.vs = 0;
    rect.vw = LCD_PIXEL_HEIGHT;
    rect.hs = 0;
    rect.hw = LCD_PIXEL_WIDTH;
    Display.Graphics_Read_Setting(
        DisplayBase::GRAPHICS_LAYER_0,
        (void *)user_frame_buffer0,
        FRAME_BUFFER_STRIDE,
        DisplayBase::GRAPHICS_FORMAT_RGB565,
        DisplayBase::WR_RD_WRSWA_32_16BIT,
        &rect
    );
    Display.Graphics_Start(DisplayBase::GRAPHICS_LAYER_0);

    ThisThread::sleep_for(50);
    EasyAttach_LcdBacklight(true);
}

void setPixel(int x, int y) {
    int i = 0;
    i = FRAME_BUFFER_STRIDE * y + x * 2;
    user_frame_buffer0[i + 0] = 0xFF; // GB
    user_frame_buffer0[i + 1] = 0xFF; // RG

    // Data cache clean
    dcache_clean(user_frame_buffer0, sizeof(user_frame_buffer0));
}

int main() {
    EasyAttach_Init(Display);
    Start_LCD_Display();

    while (1) {
        setPixel(240,160);
        dcache_clean(user_frame_buffer0, sizeof(user_frame_buffer0));
    }
}
