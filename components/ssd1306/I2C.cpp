#include "I2C.h"
#include <esp_log.h>
#include <utility>

static constexpr const signed int Delay      = -1;
static constexpr const int        BufferSize = 1;

I2C::I2C(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins)
    : Config(std::move(&Configuration)), Pins(std::move(&Pins))
{
    if (!Configure())
    {
        SetStatus(Status::ERROR);
        ESP_LOGW(I2C_TAG, "ERROR killing self");
    }
    else
    {
        SetStatus(Status::OK);
        ESP_LOGW(I2C_TAG, "SUCCESS starting");
        Start();
    }
}

bool I2C::Configure()
{
    bool Configured = true;

    if (!BuildConfig())
    {
        ESP_LOGW(I2C_TAG, "ERROR building config");
        Configured = false;
    }

    return Configured;
}

bool I2C::BuildConfig()
{
    DeviceConfig = {};
    BusConfig    = {};

    BusConfig.i2c_port                     = Config->I2C_PORT;
    BusConfig.sda_io_num                   = Pins->SDA;
    BusConfig.scl_io_num                   = Pins->SCL;
    BusConfig.clk_source                   = I2C_CLK_SRC_DEFAULT;
    BusConfig.glitch_ignore_cnt            = Config->GLITCH_COUNT;
    BusConfig.flags.enable_internal_pullup = true;

    DeviceConfig.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    DeviceConfig.device_address  = Config->ADDRESS;
    DeviceConfig.scl_speed_hz    = Config->CLOCK_SPEED_HZ;

    return ValidateConfig();
}

bool I2C::ValidateConfig()
{
    bool Valid = true;

    ESP_ERROR_CHECK(i2c_new_master_bus(&BusConfig, &BusHandle));
    ESP_ERROR_CHECK(i2c_master_bus_add_device(BusHandle, &DeviceConfig, &DeviceHandle));

    if (!BusConfig.sda_io_num)
    {
        ESP_LOGW(I2C_TAG, "ERROR failed to provide SDA pin");
        Valid = false;
    }
    else if (!BusConfig.scl_io_num)
    {
        ESP_LOGW(I2C_TAG, "ERROR failed to provide SCL pin");
        Valid = false;
    }

    return Valid;
}

void I2C::Start() const
{
    WriteByte(Commands.SET_DISPLAY_OFF);
    WriteByte(Commands.SET_DISPLAY_CLOCK_FREQ);
    WriteByte(Commands.SYS_CALL);
    WriteByte(Commands.SET_MULTIPLEX_RATIO);
    WriteByte(0x3F);
    WriteByte(Commands.SET_DISPLAY_OFFSET);
    WriteByte(ControlBytes.DUMMY_BYTE);
    WriteByte(Commands.SET_DISPLAY_START_LINE);
    WriteByte(Commands.SET_SEGMENT_REMAP_127);
    WriteByte(Commands.SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED);
    WriteByte(Commands.SET_COM_PINS_HARDWARE_CONFIG);
    WriteByte(0x12);
    WriteByte(Commands.SET_CONTRAST_CONTROL);
    WriteByte(Commands.DEFAULT_CONTRAST);
    WriteByte(Commands.SET_PRECHARGE_PERIOD);
    WriteByte((Commands.PHASE_TWO << 4 ) | Commands.PHASE_ONE);
    WriteByte(Commands.SET_VCOM_DESELECT_LEVEL);
    WriteByte(0x40);
    WriteByte(Commands.CHARGE_PUMP_SETTING);
    WriteByte(Commands.ENABLE_CHARGE_PUMP);
    WriteByte(Commands.SET_MEMORY_ADDRESSING_MODE);
    WriteByte(Commands.PAGE_ADDRESSING_MODE);
    WriteByte(Commands.SET_LOWER_COLUMN_START + 0x00);
    WriteByte(Commands.SET_HIGHER_COLUMN_START + 0x00);
    WriteByte(Commands.SET_PAGE_START | 0x00);
    WriteByte(Commands.ENTIRE_DISPLAY_ON_WITH_CONTENT);
    WriteByte(Commands.SET_NORMAL_DISPLAY);
    WriteByte(Commands.DEACTIVATE_SCROLL);
    WriteByte(Commands.SET_DISPLAY_ON);
}

[[gnu::cold]] bool I2C::Probe()
{
    static constexpr const uint8_t AddressMax = 127;
    int Found = 0;

    ESP_LOGI(I2C_TAG, "Probe : Begin");
    ESP_LOGI(I2C_TAG, "Probe : Scanning port %d  : ", Config->I2C_PORT);

    for (int AddressToScan = 0; AddressToScan < AddressMax; ++AddressToScan)
    {
        const uint8_t Address = AddressToScan;
        if (i2c_master_probe(BusHandle, AddressToScan, -1) == ESP_OK)
        {
            ESP_LOGI(I2C_TAG, "Probe : Device Found : %d ", Address);
            Found++;
        }
    }

    return Found > 0;
}

[[gnu::hot]] void I2C::Draw(const uint8_t Segment, const uint8_t Page, uint8_t Offset, size_t Bytes, uint8_t* Data) const noexcept
{
    IndexGDDRAM(Segment, Page, Offset);
    Mode(ControlBytes.DATA_MODE);
}

[[gnu::hot]] void I2C::Draw(const uint8_t SegmentLow, const uint8_t SegmentHigh, const uint8_t Page, const uint8_t Offset, const size_t Bytes, uint8_t* Data) const noexcept
{
    const uint8_t Segment = 0;
    IndexGDDRAM(Segment, Page, Offset);
    Mode(ControlBytes.DATA_MODE);
}

[[gnu::hot]] void I2C::Scroll(const Direction Direction, const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept
{
    Mode(ControlBytes.COMMAND_MODE);

    switch (Direction)
    {
        case Direction::HORIZONTAL:
            ScrollHorizontal(ScrollCommand);
            break;
        case Direction::VERTICAL:
            ScrollVertical(ScrollCommand, VerticalOffset);
            break;
        default:
            WriteByte(Commands.DEACTIVATE_SCROLL);
            break;
    }
}

void I2C::ScrollHorizontal(const uint8_t CommandByte) const noexcept
{
    WriteByte(CommandByte);
    WriteByte(ControlBytes.DUMMY_BYTE);
    WriteByte(0x00);
    WriteByte(0x07);
    WriteByte(0x07);
    WriteByte(0x00);
    WriteByte(0xFE);
    WriteByte(Commands.ACTIVATE_SCROLL);
}

void I2C::ScrollVertical(const uint8_t CommandByte, const uint8_t VerticalOffset) const noexcept
{
    WriteByte(CommandByte);
    WriteByte(ControlBytes.DUMMY_BYTE);
    WriteByte(0x00);
    WriteByte(0x07);
    WriteByte(0x00);
    WriteByte(VerticalOffset);
    WriteByte(Commands.SET_VERTICAL_SCROLL_AREA);
    WriteByte(ControlBytes.DUMMY_BYTE);
    WriteByte(Commands.SET_DISPLAY_START_LINE);
    WriteByte(Commands.ACTIVATE_SCROLL);
}

void I2C::Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept
{
    uint8_t* DummyBuffer = 0;
    Draw(Segment, Page, Offset, 0, DummyBuffer);
}

void I2C::Flush() const noexcept
{
    uint8_t* DummyBuffer[8] = {0};
    int Column = 0;
    int Columns = Config->HEIGHT + 1;

    for (int Row = 0; Column < Columns; Row++)
    {
        static constexpr size_t Size = 8;
        if ((Row >> 3) << 3 == Row)
        {
            Row = 0;
            Column++;
        }
        Draw(Column, Row, 0, Size, *DummyBuffer);
    }
}

inline void I2C::WriteByte(uint8_t Data) const noexcept
{
    i2c_master_transmit(DeviceHandle, &Data, BufferSize, Delay);
}

inline void I2C::WriteBuffer(uint8_t* Data, size_t Size) const noexcept
{
    i2c_master_transmit(DeviceHandle, Data, Size, Delay);
}

inline void I2C::ReadBuffer(const uint8_t* WriteData, size_t WriteSize, uint8_t* ReadData, size_t ReadSize) const noexcept
{
    i2c_master_transmit_receive(DeviceHandle, WriteData, WriteSize, ReadData, ReadSize, Delay);
}

void I2C::Mode(const uint8_t CommandByte) const noexcept
{
    WriteByte(CommandByte);
}

void I2C::IndexGDDRAM(const uint8_t Segment, const uint8_t Page, uint8_t Offset) const noexcept
{
    const uint8_t SegmentLow = OffsetLow(Segment, Offset);
    const uint8_t SegmentHigh = OffsetHigh(Segment, Offset);
    WriteByte(Commands.SET_LOWER_COLUMN_START + SegmentLow);
    WriteByte(Commands.SET_HIGHER_COLUMN_START + SegmentHigh);
    WriteByte(Commands.SET_PAGE_START | Page);
}