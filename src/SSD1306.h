#ifndef SSD1306_3D_DEVICE_DRIVER_SSD1306_H
#define SSD1306_3D_DEVICE_DRIVER_SSD1306_H

#include "ControlBytes.h"
#include "Commands.h"
#include "Pins.h"
#include "Configuration.h"
#include "I2C.h"
#include <driver/i2c.h>

class SSD1306
{
    public:

    SSD1306(uint16_t Width, uint16_t Height, uint8_t Address, SSD1306ControlBytes ControlBytes, SSD1306Commands Commands, SSD1306Pins Pins, SSD1306Configuration Configuration);

    [[gnu::hot]] void Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Width, const uint8_t Offset, uint8_t& Data) const noexcept;
    [[nodiscard]] inline uint8_t GetWidth() const noexcept{return this->Width;}
    [[nodiscard]] inline uint8_t GetHeight() const noexcept{return this->Height;}

    private:

    bool Init(SSD1306ControlBytes ControlBytes, SSD1306Commands Commands, SSD1306Pins Pins, SSD1306Configuration Configuration) noexcept;

    I2C*     Bus = nullptr;
    uint16_t Width;
    uint16_t Height;
    uint8_t  Address;
};

#endif