#ifndef SSD1306_SPI_H
#define SSD1306_SPI_H

#include "BusProtocol.h"
#include <driver/spi_master.h>

#define SPI_TAG "SPI"

class SPI final : public BusProtocol
{
public:
    SPI(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins);

    [[gnu::cold]] bool Probe() override;
    [[gnu::hot]] void Draw(const uint8_t Segment, const uint8_t Page, uint8_t Offset, size_t Bytes, uint8_t* Data) const noexcept override;
    [[gnu::hot]] void Draw(const uint8_t SegmentLow, const uint8_t SegmentHigh, const uint8_t Page, const uint8_t Offset, const size_t Bytes, uint8_t* Data) const noexcept override;
    [[gnu::hot]] void Scroll(const Direction Direction, const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept override;
    void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept override;
    void Flush() const noexcept override;

private:
    const SSD1306Configuration* Config;
    const SSD1306Pins*          Pins;
    spi_bus_config_t            BusConfig  = {};
    spi_device_handle_t         DeviceHandle = nullptr;
    SSD1306ControlBytes         ControlBytes;
    SSD1306Commands             Commands;

    bool Configure();
    bool Init();
    void SetupPin(gpio_num_t Pin) const noexcept;
    [[nodiscard]] spi_device_handle_t SetupDevice() const noexcept;
    [[nodiscard]] bool BuildConfig();
    [[nodiscard]] bool ValidateConfig() const;
    void Start() const noexcept;

    bool WriteByte(const int Data, uint8_t CommandByte, uint8_t Pin) const noexcept;
    bool WriteBuffer(const uint8_t* Data, size_t Bytes, uint8_t CommandByte, uint8_t Pin) const noexcept;
    void Mode(const uint8_t CommandByte, const uint8_t Pin) const noexcept;
    void IndexGDDRAM(const uint8_t Segment, const uint8_t Page, uint8_t Offset) const noexcept;
    void IndexGDDRAM(const uint8_t SegmentLow, const uint8_t SegmentHigh, const uint8_t Page, const uint8_t Offset) const noexcept;
    void ScrollHorizontal(const uint8_t ScrollCommand) const noexcept;
    void ScrollVertical(const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept;
};

#endif