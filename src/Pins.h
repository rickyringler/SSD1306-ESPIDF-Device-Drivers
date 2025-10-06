#ifndef SSD1306_3D_DEVICE_DRIVER_PINS_H
#define SSD1306_3D_DEVICE_DRIVER_PINS_H
#include <hal/gpio_types.h>

struct SSD1306Pins
{
    static constexpr int SDA        = 21;
    static constexpr int SCL        = 22;
    static constexpr int SCLK       = GPIO_NUM_5;
    static constexpr int MOSI       = GPIO_NUM_18;
    static constexpr int MISO       = -1;
    gpio_num_t DC                   = GPIO_NUM_33;
    gpio_num_t CS                   = GPIO_NUM_27;
};

#endif//SSD1306_3D_DEVICE_DRIVER_PINS_H