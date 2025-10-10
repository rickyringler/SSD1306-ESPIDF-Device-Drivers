#ifndef SSD1306_3D_DEVICE_DRIVER_SSD1306_H
#define SSD1306_3D_DEVICE_DRIVER_SSD1306_H

#include "Configuration.h"
#include "BusProtocol.h"
#include <driver/i2c.h>

class SSD1306
{
public:

    SSD1306(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins, Protocol Protocol);

    [[gnu::cold]] void Probe() const;
    [[gnu::hot]] void Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Offset, size_t Bytes, uint8_t* Data) const noexcept;
    [[gnu::hot]] void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept;
    [[gnu::hot]] void Flush(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept;

private:

    [[gnu::cold]] bool Init(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins, Protocol Protocol) noexcept;

    BusProtocol*     Bus = nullptr;
};

#endif