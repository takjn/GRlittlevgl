#include "mbed.h"
#include "dcache-control.h"
#include "EasyAttach_CameraAndLCD.h"
#include "lvgl/lvgl.h"
#include "lv_port_indev.h"

// Demo program
#include "lv_examples/lv_examples.h"
#include "lv_examples/lv_apps/demo/demo.h"
#include "lvgl/src/lv_themes/lv_theme_material.h"
#include "lv_examples/lv_tests/lv_test_theme/lv_test_theme_1.h"

#define FRAME_BUFFER_STRIDE    (((LCD_PIXEL_WIDTH * 2) + 31u) & ~31u)
#define FRAME_BUFFER_HEIGHT    (LCD_PIXEL_HEIGHT)

static uint8_t user_frame_buffer0[FRAME_BUFFER_STRIDE * FRAME_BUFFER_HEIGHT]__attribute((aligned(32)));

DisplayBase Display;

static void disp_init(void) {
    // Initialize the background to black
    for (uint32_t i = 0; i < sizeof(user_frame_buffer0); i += 2) {
        user_frame_buffer0[i + 0] = 0x00;
        user_frame_buffer0[i + 1] = 0x00;
    }
    // Data cache clean
    dcache_clean(user_frame_buffer0, sizeof(user_frame_buffer0));

    DisplayBase::rect_t rect;
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

void set_pixel(int x, int y, lv_color_t color) {
    int i = FRAME_BUFFER_STRIDE * y + x * 2;
    user_frame_buffer0[i + 0] = (color.full & 0xFF); // GB
    user_frame_buffer0[i + 1] = (color.full >> 8); // RG
}

Ticker ticker;
void lv_tick_inc_handler() {
    lv_tick_inc(1);
    lv_task_handler();
}

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    // The most simple case (but also the slowest) to put all pixels to the screen one-by-one
    int32_t x;
    int32_t y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            set_pixel(x, y, *color_p);  /* Put a pixel to the display.*/
            color_p++;
        }
    }

    // Data cache clean
    dcache_clean(user_frame_buffer0, sizeof(user_frame_buffer0));

    // Inform the graphics library that you are ready with the flushing
    lv_disp_flush_ready(disp_drv);
}

void lv_port_disp_init(void)
{
    // Initialize Gadged Renesas display
    disp_init();

    // Create a buffer for drawing
    static lv_disp_buf_t disp_buf_2;
    static lv_color_t buf2_1[LV_HOR_RES_MAX * 10];                        /*A buffer for 10 rows*/
    static lv_color_t buf2_2[LV_HOR_RES_MAX * 10];                        /*An other buffer for 10 rows*/
    lv_disp_buf_init(&disp_buf_2, buf2_1, buf2_2, LV_HOR_RES_MAX * 10);   /*Initialize the display buffer*/

    // Register the display in LittlevGL
    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    // Set the resolution of the display
    disp_drv.hor_res = LCD_PIXEL_WIDTH;
    disp_drv.ver_res = LCD_PIXEL_HEIGHT;

    // Used to copy the buffer's content to the display
    disp_drv.flush_cb = disp_flush;

    // Set a display buffer
    disp_drv.buffer = &disp_buf_2;

    // Finally register the driver
    lv_disp_drv_register(&disp_drv);
}

// Initialize LittlevGL
void littlevgl_init() {
    ticker.attach_us(&lv_tick_inc_handler, 1000); // 1 ms
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
}

int main() {
    EasyAttach_Init(Display);

    littlevgl_init();

#if (1)
    // Start LittlevGL demo
    demo_create();
#else
    // Test LittlevGL theme
    uint16_t hue = 0;
    lv_test_theme_1(lv_theme_material_init(hue, NULL));
#endif

    wait(osWaitForever);
}
