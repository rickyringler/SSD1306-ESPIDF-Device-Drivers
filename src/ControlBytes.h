#ifndef SSD1306_3D_DEVICE_DRIVER_CONTROLBYTES_H
#define SSD1306_3D_DEVICE_DRIVER_CONTROLBYTES_H

struct SSD1306ControlBytes
{
    static constexpr int COMMAND_MODE = 0x00;
    static constexpr int DATA_MODE    = 0x40;
};

#endif