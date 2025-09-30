#ifndef SSD1306_3D_DEVICE_DRIVER_PINS_H
#define SSD1306_3D_DEVICE_DRIVER_PINS_H
#include <hal/gpio_types.h>

struct SSD1306Pins
{
    static constexpr int SDA        = 21;
    static constexpr int SCL        = 22;
    static constexpr int SCLK       = 0;
    static constexpr int MOSI       = 0;
    static constexpr int MISO       = 1;
    gpio_num_t DC;
    gpio_num_t CS;
};

#endif//SSD1306_3D_DEVICE_DRIVER_PINS_H