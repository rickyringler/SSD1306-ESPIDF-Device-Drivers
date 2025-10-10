#ifndef SSD1306_3D_DEVICE_DRIVER_I2C_H
#define SSD1306_3D_DEVICE_DRIVER_I2C_H

#include "BusProtocol.h"

#define I2C_TAG "I2C"

class I2C final : public BusProtocol
{
public:

    I2C(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins);

    [[gnu::cold]] esp_err_t Probe() const override;
    [[gnu::hot]] void Draw(const uint8_t Segment, const uint8_t Page, uint8_t Offset, size_t Bytes, uint8_t* Data) const noexcept override;
    [[gnu::hot]] void Scroll(const Direction Direction, const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept override;
    [[gnu::hot]] void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept override;
    [[gnu::hot]] void Flush() const noexcept override;

private:

    const SSD1306Configuration* DeviceConfig;
    const SSD1306Pins*          Pins;
    i2c_config_t                BusConfig  = {};
    uint8_t                     Address;
    SSD1306ControlBytes         ControlBytes;
    SSD1306Commands             Commands;

    bool                    Configure() noexcept;
    bool                    Start() const noexcept;
    [[gnu::hot]]  void      Mode(const uint8_t Mode, i2c_cmd_handle_t& CommandHandle) const noexcept;
    [[nodiscard]] esp_err_t BuildConfig() noexcept;
    [[nodiscard]] bool      ValidateConfig() const noexcept;
    [[nodiscard]] uint8_t   OffsetLow(uint8_t Segment, uint8_t Offset) const noexcept;
    [[nodiscard]] uint8_t   OffsetHigh(uint8_t Segment, uint8_t Offset) const noexcept;
    [[gnu::hot]]  void      IndexGDDRAM(const uint8_t Segment, const uint8_t Page, uint8_t Offset) const noexcept;
    [[gnu::hot]]  void      ScrollHorizontal(const uint8_t CommandByte, i2c_cmd_handle_t& Command) const noexcept;
    [[gnu::hot]]  void      ScrollVertical(const uint8_t CommandByte, i2c_cmd_handle_t& Command, const uint8_t VerticalOffset) const noexcept;
};

#endif
