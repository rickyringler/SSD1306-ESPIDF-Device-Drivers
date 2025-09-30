#ifndef SSD1306_3D_DEVICE_DRIVER_SPI_H
#define SSD1306_3D_DEVICE_DRIVER_SPI_H

#include "BusProtocol.h"

#define TAG "I2C"
#define TIMEOUT (1000/portTICK_PERIOD_MS)

class SPI final : public BusProtocol
{
public:

    SPI(uint8_t Address, const SSD1306Configuration& Configuration);

    [[gnu::cold]] void BusAddressScan() const override;
    [[gnu::hot]] void Draw(const uint8_t Segment, const uint8_t Page, uint8_t Width, uint8_t Offset, uint8_t Data) const noexcept override;
    [[gnu::hot]] void Scroll(const Direction Direction, const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept override{}
    [[gnu::hot]] void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Width, const uint8_t Offset) const noexcept override;
    [[gnu::hot]] void Flush() const noexcept override;

private:

    spi_bus_config_t     BusConfig  = {};
    spi_device_handle_t* DeviceHandle = nullptr;
    uint8_t              Address;
    SSD1306ControlBytes  ControlBytes;
    SSD1306Commands      Commands;
    SSD1306Pins          Pins;
    SSD1306Configuration DeviceConfig;

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
    [[gnu::hot]]  void      ScrollHorizontal(const uint8_t CommandByte, i2c_cmd_handle_t& Command) const noexcept{}
    [[gnu::hot]]  void      ScrollVertical(const uint8_t CommandByte, i2c_cmd_handle_t& Command, const uint8_t VerticalOffset) const noexcept{}
    [[nodiscard]] spi_device_handle_t* SetupDevice() const noexcept;

    [[gnu::hot]] bool WriteByte(const int Data, const uint8_t Width, uint8_t CommandByte, uint8_t Pin) const noexcept;
};

#endif