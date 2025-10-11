#include "SSD1306.h"
#include "I2C.h"
#include "SPI.h"

SSD1306::SSD1306(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins, Protocol Protocol)
{
    Init(Configuration, Pins, Protocol);
}

[[gnu::cold]] void SSD1306::Probe() const
{
    this->Bus->Probe();
}

[[gnu::cold]] bool SSD1306::Init(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins, Protocol Protocol)
{
    switch (Protocol)
    {
        case SPI_BUS:
            this->Bus = new SPI(Configuration, Pins);
            return true;
        case I2C_BUS:
            this->Bus = new I2C(Configuration, Pins);
            return true;
        default:
            break;
    }
    return false;
}

[[gnu::hot]] void SSD1306::Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Offset, const size_t Bytes, uint8_t* Data) const noexcept
{
    this->Bus->Draw(Segment, Page, Offset, Bytes, Data);
}

[[gnu::hot]] void SSD1306::Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept
{
    this->Bus->Clear(Segment, Page, Offset);
}
[[gnu::hot]] void SSD1306::Flush(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept
{
    this->Bus->Flush();
}