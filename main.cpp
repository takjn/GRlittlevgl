#include "mbed.h"
#include "lvgl/lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

// Demo program
#include "lv_examples/lv_examples.h"
#include "lv_examples/lv_apps/demo/demo.h"
#include "lvgl/src/lv_themes/lv_theme_material.h"
#include "lv_examples/lv_tests/lv_test_theme/lv_test_theme_1.h"

// Initialize LittlevGL
static void littlevgl_init() {
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
}

int main() {
    // Initialize LittlevGL
    littlevgl_init();

#if (0)
    // Start LittlevGL demo
    demo_create();
#else
    // Test LittlevGL theme
    uint16_t hue = 0;
    lv_test_theme_1(lv_theme_material_init(hue, NULL));
#endif

    wait(osWaitForever);
}
