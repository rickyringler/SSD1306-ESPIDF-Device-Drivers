#ifndef SSD1306_3D_DEVICE_DRIVER_SSD1306_H
#define SSD1306_3D_DEVICE_DRIVER_SSD1306_H

#include "Configuration.h"
#include "BusProtocol.h"
#include <driver/i2c.h>

class SSD1306
{
public:

    SSD1306(const uint16_t Width, const uint16_t Height, const uint8_t Pages, const uint8_t Address, SSD1306Configuration& Configuration);

    [[gnu::cold]] void Probe() const;
    [[gnu::hot]] void Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Offset, size_t Bytes, uint8_t* Data) const noexcept;
    [[gnu::hot]] void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept;
    [[gnu::hot]] void Flush() const noexcept;
    [[nodiscard]] inline uint8_t GetWidth() const noexcept{return this->Width;}
    [[nodiscard]] inline uint8_t GetHeight() const noexcept{return this->Height;}
    [[nodiscard]] inline uint8_t GetPages() const noexcept{return this->Pages;}

private:

    [[gnu::cold]] bool Init(const uint8_t Address, SSD1306Configuration Configuration) noexcept;

    BusProtocol*     Bus = nullptr;
    uint16_t Width;
    uint16_t Height;
    uint8_t  Address;
    uint8_t  Pages;
};

#endif