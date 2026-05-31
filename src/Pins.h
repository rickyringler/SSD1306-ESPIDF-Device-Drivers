#ifndef SSD1306_PINS_H
#define SSD1306_PINS_H
#include <hal/gpio_types.h>

struct SSD1306Pins
{
    SSD1306Pins()=default;

    SSD1306Pins(const SSD1306Pins& Other)=delete;

    SSD1306Pins(gpio_num_t SCLK, gpio_num_t MOSI, gpio_num_t DC, gpio_num_t CS)
    :SCLK(SCLK), MOSI(MOSI), DC(DC), CS(CS){}

    static constexpr int SDA        = 21;
    static constexpr int SCL        = 22;
    const int SCLK                  = GPIO_NUM_5;
    const int MOSI                  = GPIO_NUM_18;
    static constexpr int MISO       = -1;
    gpio_num_t DC                   = GPIO_NUM_33;
    gpio_num_t CS                   = GPIO_NUM_27;
};

#endif