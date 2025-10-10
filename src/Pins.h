#ifndef SSD1306_3D_DEVICE_DRIVER_PINS_H
#define SSD1306_3D_DEVICE_DRIVER_PINS_H
#include <hal/gpio_types.h>

struct SSD1306Pins
{
    SSD1306Pins(gpio_num_t SCLK, gpio_num_t MOSI, gpio_num_t DC, gpio_num_t CS)
    :SCLK(SCLK), MOSI(MOSI), DC(DC), CS(CS){}

#pragma region I2C
    static constexpr int SDA        = 21;
    static constexpr int SCL        = 22;
#pragma endregion
#pragma region SPI
    const int SCLK                  = GPIO_NUM_5;
    const int MOSI                  = GPIO_NUM_18;
    static constexpr int MISO       = -1;
    gpio_num_t DC                   = GPIO_NUM_33;
    gpio_num_t CS                   = GPIO_NUM_27;
#pragma endregion
};

#endif//SSD1306_3D_DEVICE_DRIVER_PINS_H