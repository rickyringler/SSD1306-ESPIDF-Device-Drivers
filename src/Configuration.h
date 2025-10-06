#ifndef SSD1306_3D_DEVICE_DRIVER_CONFIGURATION_H
#define SSD1306_3D_DEVICE_DRIVER_CONFIGURATION_H

struct SSD1306Configuration
{
    const int CLOCK_SPEED_HZ=400000;
    const int I2C_MODE_MASTER=1;
    const int GPIO_PULLUP_ENABLE=1;
    const int PAGES=8;
    const int BIT_SIZE=8;
};

#endif