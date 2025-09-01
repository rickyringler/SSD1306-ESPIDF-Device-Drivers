#ifndef SSD1306_3D_DEVICE_DRIVER_I2C_H
#define SSD1306_3D_DEVICE_DRIVER_I2C_H

#include "SSD1306.h"
#include "Commands.h"
#include "ControlBytes.h"
#include "Pins.h"
#include <driver/i2c.h>

#define TAG "I2C"

class I2C
{

public:

    I2C(uint8_t Address, const SSD1306Configuration& Configuration);

    [[gnu::hot]] void Draw(const uint8_t Segment, const uint8_t Page, uint8_t Width, uint8_t Offset, uint8_t Data[16]) const noexcept;
    [[gnu::hot]] void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Width, const uint8_t Offset) const noexcept;
    [[gnu::hot]] void Flush() const noexcept;

private:

    i2c_config_t         BusConfig  = {};
    uint8_t              Address;
    SSD1306ControlBytes  ControlBytes;
    SSD1306Commands      Commands;
    SSD1306Pins          Pins;
    SSD1306Configuration DeviceConfig;

    bool                    Configure() noexcept;
    bool                    Start() const noexcept;
    [[gnu::hot]]  void      Mode(const uint8_t Mode, i2c_cmd_handle_t& CommandHandle) const noexcept;
    [[nodiscard]] esp_err_t BuildConfig() noexcept;
    [[nodiscard]] bool      ValidateConfig() const noexcept;
    [[nodiscard]] uint8_t   OffsetLow(uint8_t Segment, uint8_t Offset) const noexcept;
    [[nodiscard]] uint8_t   OffsetHigh(uint8_t Segment, uint8_t Offset) const noexcept;
    [[gnu::hot]]  void      IndexGDDRAM(const uint8_t Segment, const uint8_t Page, uint8_t Offset) const noexcept;
};

#endif
