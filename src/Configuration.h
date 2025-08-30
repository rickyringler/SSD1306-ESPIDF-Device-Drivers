#ifndef SSD1306_3D_DEVICE_DRIVER_CONFIGURATION_H
#define SSD1306_3D_DEVICE_DRIVER_CONFIGURATION_H

struct SSD1306Configuration
{
    static constexpr int CLOCK_SPEED_HZ = 400000;
    static constexpr int I2C_MODE_MASTER = 1;
    static constexpr int GPIO_PULLUP_ENABLE = 1;
};

#endif