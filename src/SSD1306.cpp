#include "SSD1306.h"

SSD1306::SSD1306(const uint16_t Width, const uint16_t Height, const uint8_t Address, const SSD1306ControlBytes ControlBytes, const SSD1306Commands Commands, const SSD1306Pins Pins, const SSD1306Configuration Configuration)
: Width(Width), Height(Height), Address(Address)
{
    Init(ControlBytes, Commands, Pins, Configuration);
}

bool SSD1306::Init(const SSD1306ControlBytes ControlBytes, const SSD1306Commands Commands, const SSD1306Pins Pins, const SSD1306Configuration Configuration) noexcept
{
    this->Bus = new I2C(this->Address, ControlBytes, Commands, Pins, Configuration);
    return true;
}

[[gnu::hot]] void SSD1306::Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Width, const uint8_t Offset, uint8_t& Data) const noexcept
{
    this->Bus->Draw(Segment, Page, Width, Offset, Data);
}