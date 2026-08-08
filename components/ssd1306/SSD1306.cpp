#include "SSD1306.h"
#include "I2C.h"
#include "SPI.h"
#include <esp_log.h>

SSD1306::SSD1306(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins, Protocols Protocol)
    : Protocol(Protocol)
{
    Init(Configuration, Pins, Protocol);
}

[[gnu::cold]][[nodiscard]] bool SSD1306::Probe() const
{
   return this->Bus->Probe();
}

bool SSD1306::Init(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins, Protocols Protocol)
{
    switch (Protocol)
    {
        case Protocols::SPI_BUS:
            this->Bus = new SPI(Configuration, Pins);
            return true;
        case Protocols::I2C_BUS:
            this->Bus = new I2C(Configuration, Pins);
            return true;
        case Protocols::UART_BUS:
            assert("UART protocol not supported for this SSD1306 driver.");
            return false;
        default:
            ESP_LOGW(Tag, "ERROR : expected bus protocol, but you passed in nonsense.");
            break;
    }
    return false;
}

[[gnu::hot]] void SSD1306::Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Offset, const size_t Bytes, uint8_t* Data) const noexcept
{
    this->Bus->Draw(Segment, Page, Offset, Bytes, Data);
}

[[gnu::hot]] void SSD1306::Draw(const uint8_t SegmentLow, const uint8_t SegmentHigh, const uint8_t Page, const uint8_t Offset, const size_t Bytes, uint8_t* Data) const noexcept
{
    this->Bus->Draw(SegmentLow, SegmentHigh, Page, Offset, Bytes, Data);
}

[[gnu::hot]] void SSD1306::Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept
{
    this->Bus->Clear(Segment, Page, Offset);
}

[[gnu::hot]] void SSD1306::Flush() const noexcept
{
    this->Bus->Flush();
}

[[nodiscard]] Statuses SSD1306::GetStatus() const noexcept
{
    return this->Bus->GetStatus();
}