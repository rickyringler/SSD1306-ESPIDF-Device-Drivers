#ifndef SSD1306_3D_DEVICE_DRIVER_BUSPROTOCOL_H
#define SSD1306_3D_DEVICE_DRIVER_BUSPROTOCOL_H

#include "Configuration.h"
#include "Commands.h"
#include "ControlBytes.h"
#include "Pins.h"
#include "Types.h"
#include <driver/i2c.h>
#include <driver/spi_master.h>

class BusProtocol
{
public:

    virtual ~BusProtocol() = default;

    [[gnu::cold]] virtual esp_err_t Probe() const=0;
    [[gnu::hot]] virtual void Draw(const uint8_t Segment, const uint8_t Page, uint8_t Width, uint8_t Offset, uint8_t Data) const noexcept=0;
    [[gnu::hot]] virtual void Scroll(const Direction Direction, const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept=0;
    [[gnu::hot]] virtual void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Width, const uint8_t Offset) const noexcept=0;
    [[gnu::hot]] virtual void Flush() const noexcept=0;
};

#endif
