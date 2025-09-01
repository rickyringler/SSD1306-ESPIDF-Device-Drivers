#include "SSD1306.h"

SSD1306::SSD1306(const uint16_t Width, const uint16_t Height, const uint8_t Pages, const uint8_t Address, const SSD1306Configuration& Configuration)
: Width(Width), Height(Height), Address(Address), Pages(Pages)
{
    Init(Configuration);
}

[[gnu::cold]] bool SSD1306::Init(const SSD1306Configuration& Configuration) noexcept
{
    this->Bus = new I2C(this->Address, Configuration);
    return true;
}

[[gnu::hot]] void SSD1306::Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Offset, uint8_t Data[16]) const noexcept
{
    this->Bus->Draw(Segment, Page, Width, Offset, Data);
}

[[gnu::hot]] void SSD1306::Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept
{
    this->Bus->Clear(Segment, Page, Width, Offset);
}
[[gnu::hot]] void SSD1306::Flush() const noexcept
{

}