#ifndef SSD1306_BUSPROTOCOL_H
#define SSD1306_BUSPROTOCOL_H

#include "Configuration.h"
#include "Commands.h"
#include "ControlBytes.h"
#include "Pins.h"
#include "Types.h"
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"

#define TIMEOUT (1000/portTICK_PERIOD_MS)

/*
* Pure virtual to be inherited by protocol implementation.
*
* This enforces a contract between the device and the protocol, so that
* the device need not know anything about the protocol and can just call
* obviously named functions like Draw().
*/

class BusProtocol
{
public:
    virtual ~BusProtocol()=default;

    [[gnu::cold]] virtual bool Probe()=0;
    [[gnu::hot]] virtual void Draw(const uint8_t Segment, const uint8_t Page, uint8_t Offset, size_t Bytes, uint8_t* Data) const noexcept=0;
    [[gnu::hot]] virtual void Draw(const uint8_t SegmentLow, const uint8_t SegmentHigh, const uint8_t Page, const uint8_t Offset, const size_t Bytes, uint8_t* Data) const noexcept=0;
    [[gnu::hot]] virtual void Scroll(const Direction Direction, const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept=0;
    [[gnu::hot]] virtual void Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept=0;
    [[gnu::hot]] virtual void Flush() const noexcept=0;
    [[nodiscard]] Status GetStatus() const noexcept { return this->DeviceStatus;}

protected:
    Status DeviceStatus = Status::NONE;

	[[nodiscard]] uint8_t OffsetLow(uint8_t Segment, uint8_t Offset) const noexcept
	{
    	const uint8_t OffsetLow = (Segment + Offset) & 0x0F;
    	return OffsetLow;
	}

	[[nodiscard]] uint8_t OffsetHigh(uint8_t Segment, uint8_t Offset) const noexcept
	{
    	const uint8_t OffsetHigh = ((Segment + Offset) >> 4) & 0x0F;
    	return OffsetHigh;
	}

    inline void SetStatus(Status NewDeviceStatus) noexcept
	{
		this->DeviceStatus = NewDeviceStatus;
	}
};

#endif
