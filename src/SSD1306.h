#ifndef SSD1306_3D_DEVICE_DRIVER_SSD1306_H
#define SSD1306_3D_DEVICE_DRIVER_SSD1306_H

#include "Configuration.h"
#include "I2C.h"
#include <driver/i2c.h>

class SSD1306
{
    public:

    SSD1306(uint16_t Width, uint16_t Height, uint8_t Pages, uint8_t Address, const SSD1306Configuration& Configuration);

    [[gnu::hot]] void Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Offset, uint8_t Data[16]) const noexcept;
    [[gnu::hot]] void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept;
    [[gnu::hot]] void Flush() const noexcept;
    [[nodiscard]] inline uint8_t GetWidth() const noexcept{return this->Width;}
    [[nodiscard]] inline uint8_t GetHeight() const noexcept{return this->Height;}
    [[nodiscard]] inline uint8_t GetPages() const noexcept{return this->Pages;}

    private:

    [[gnu::cold]] bool Init(const SSD1306Configuration& Configuration) noexcept;

    I2C*     Bus = nullptr;
    uint16_t Width;
    uint16_t Height;
    uint8_t  Address;
    uint8_t  Pages;
};

#endif