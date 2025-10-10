#ifndef SSD1306_3D_DEVICE_DRIVER_SPI_H
#define SSD1306_3D_DEVICE_DRIVER_SPI_H

#include "BusProtocol.h"

#define SPI_TAG "SPI"

class SPI final : public BusProtocol
{
public:

    SPI(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins);

    [[gnu::cold]] esp_err_t Probe() const override;
    [[gnu::hot]] void Draw(const uint8_t Segment, const uint8_t Page, uint8_t Offset, size_t Bytes, uint8_t* Data) const noexcept override;
    [[gnu::hot]] void Scroll(const Direction Direction, const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept override;
    [[gnu::hot]] void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept override;
    [[gnu::hot]] void Flush() const noexcept override;

private:

    const SSD1306Configuration* DeviceConfig;
    const SSD1306Pins*          Pins;
    spi_bus_config_t            BusConfig  = {};
    spi_device_handle_t         DeviceHandle = nullptr;
    SSD1306ControlBytes         ControlBytes;
    SSD1306Commands             Commands;

    bool                    Configure() noexcept;
    bool                    Init() noexcept;
    void                    SetupPin(gpio_num_t Pin) const noexcept;
    bool                    Start() const noexcept;
    [[gnu::hot]]  void      Mode(const uint8_t CommandByte, const uint8_t Pin) const noexcept;
    [[nodiscard]] esp_err_t BuildConfig() noexcept;
    [[nodiscard]] bool      ValidateConfig() const noexcept;
    [[nodiscard]] uint8_t   OffsetLow(uint8_t Segment, uint8_t Offset) const noexcept;
    [[nodiscard]] uint8_t   OffsetHigh(uint8_t Segment, uint8_t Offset) const noexcept;
    [[gnu::hot]]  void      IndexGDDRAM(const uint8_t Segment, const uint8_t Page, uint8_t Offset) const noexcept;
    [[gnu::hot]]  void      ScrollHorizontal(const uint8_t ScrollCommand) const noexcept;
    [[gnu::hot]]  void      ScrollVertical(const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept;

    [[gnu::hot]] bool WriteByte(const int Data, uint8_t CommandByte, uint8_t Pin) const noexcept;
    [[gnu::hot]] bool WriteBuffer(const uint8_t* Data, size_t Bytes, uint8_t CommandByte, uint8_t Pin) const noexcept;
    [[nodiscard]] spi_device_handle_t SetupDevice() const noexcept;
};

#endif