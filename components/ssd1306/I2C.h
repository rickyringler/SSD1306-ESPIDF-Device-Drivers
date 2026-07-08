#ifndef SSD1306_I2C_H
#define SSD1306_I2C_H

#include "BusProtocol.h"
#include <driver/i2c_master.h>

#define I2C_TAG "I2C"

class I2C final : public BusProtocol
{
public:
    I2C(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins);

    [[gnu::cold]] bool Probe() override;
    [[gnu::hot]] void Draw(const uint8_t Segment, const uint8_t Page, uint8_t Offset, size_t Bytes, uint8_t* Data) const noexcept override;
    [[gnu::hot]] void Draw(const uint8_t SegmentLow, const uint8_t SegmentHigh, const uint8_t Page, const uint8_t Offset, const size_t Bytes, uint8_t* Data) const noexcept override;
    [[gnu::hot]] void Scroll(const Direction Direction, const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept override;
    void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept override;
    void Flush() const noexcept override;

private:
    const SSD1306Configuration* Config;
    const SSD1306Pins*          Pins;
    uint8_t                     Address;
    SSD1306ControlBytes         ControlBytes;
    SSD1306Commands             Commands;
    i2c_master_dev_handle_t     DeviceHandle = {};
    i2c_master_bus_handle_t     BusHandle    = {};
    i2c_device_config_t         DeviceConfig;
    i2c_master_bus_config_t     BusConfig;

    bool Configure();
    [[nodiscard]] bool BuildConfig();
    [[nodiscard]] bool ValidateConfig();
    void Start() const noexcept;

    inline void WriteByte(uint8_t Data) const noexcept;
    inline void WriteBuffer(uint8_t* Data, size_t Size) const noexcept;
    inline void ReadBuffer(const uint8_t* WriteData, size_t WriteSize, uint8_t* ReadData, size_t ReadSize) const noexcept;
    void Mode(const uint8_t CommandByte) const noexcept;
    void IndexGDDRAM(const uint8_t Segment, const uint8_t Page, uint8_t Offset) const noexcept;
    void ScrollHorizontal(const uint8_t CommandByte) const noexcept;
    void ScrollVertical(const uint8_t CommandByte, const uint8_t VerticalOffset) const noexcept;
};

#endif
