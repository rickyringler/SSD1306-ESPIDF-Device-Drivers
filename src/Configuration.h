#ifndef SSD1306_3D_DEVICE_DRIVER_CONFIGURATION_H
#define SSD1306_3D_DEVICE_DRIVER_CONFIGURATION_H

#include <cstdint>

struct SSD1306Configuration
{
    SSD1306Configuration(const int ClockSpeedHz, const int I2cModeMaster, const int PullupEnable,  const int BitSize, const uint16_t Width, const uint16_t Height, const uint8_t Address)
    :CLOCK_SPEED_HZ(ClockSpeedHz), I2C_MODE_MASTER(I2cModeMaster), GPIO_PULLUP_ENABLE(PullupEnable), BIT_SIZE(BitSize), WIDTH(Width), HEIGHT(Height), ADDRESS(Address){}

    SSD1306Configuration(const int ClockSpeedHz, const int PullupEnable,  const int BitSize, const uint16_t Width, const uint16_t Height, const uint8_t Address)
    :CLOCK_SPEED_HZ(ClockSpeedHz), GPIO_PULLUP_ENABLE(PullupEnable), BIT_SIZE(BitSize), WIDTH(Width), HEIGHT(Height), ADDRESS(Address){}

    static constexpr int PAGES        = 8;
    const int      CLOCK_SPEED_HZ     = 400000;
    const int      I2C_MODE_MASTER    = 1;
    const int      GPIO_PULLUP_ENABLE = 1;
    const int      BIT_SIZE           = 8;
    const uint16_t WIDTH              = 64;
    const uint16_t HEIGHT             = 128;
    const uint8_t  ADDRESS            = 0xFF;
};

#endif