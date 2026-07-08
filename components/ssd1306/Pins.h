#ifndef SSD1306_PINS_H
#define SSD1306_PINS_H

#include <hal/gpio_types.h>

struct SSD1306Pins
{
    SSD1306Pins()=default;

    SSD1306Pins(gpio_num_t SDA, gpio_num_t SCL, gpio_num_t SCLK, gpio_num_t MOSI, gpio_num_t DC, gpio_num_t CS)
    :SDA(SCL), SCL(SCL), SCLK(SCLK), MOSI(MOSI), DC(DC), CS(CS){}

    SSD1306Pins(gpio_num_t SCLK, gpio_num_t MOSI, gpio_num_t DC, gpio_num_t CS)
    :SCLK(SCLK), MOSI(MOSI), DC(DC), CS(CS){}

    SSD1306Pins(gpio_num_t SDA, gpio_num_t SCL)
    :SDA(SDA), SCL(SCL){}

    gpio_num_t SDA            = GPIO_NUM_23;
    gpio_num_t SCL            = GPIO_NUM_22;
    gpio_num_t SCLK           = GPIO_NUM_5;
    gpio_num_t MOSI           = GPIO_NUM_18;
    gpio_num_t MISO           = GPIO_NUM_37;
    gpio_num_t DC             = GPIO_NUM_33;
    gpio_num_t CS             = GPIO_NUM_27;
};

#endif