#include "SPI.h"
#include <esp_log.h>
#include <utility>

SPI::SPI(const SSD1306Configuration& Configuration, const SSD1306Pins& Pins)
    : Config(std::move(&Configuration)), Pins(std::move(&Pins))
{
    if (!Configure() || !Init())
	{
		SetStatus(Status::ERROR);
    	ESP_LOGW(SPI_TAG, "ERROR killing self");
	}
	else
	{
		SetStatus(Status::OK);
    	ESP_LOGW(SPI_TAG, "SUCCESS starting");
		Start();
	}
}

bool SPI::Configure()
{
    bool Configured = true;

    if (!BuildConfig())
    {
        ESP_LOGW(SPI_TAG, "ERROR building config");
        Configured = false;
    }

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &BusConfig, SPI_DMA_CH_AUTO));
    return Configured;
}

bool SPI::Init()
{
    bool Initialized = true;

	SetupPin(Pins->DC);
    this->DeviceHandle = SetupDevice();

	if (!this->DeviceHandle)
	{
        ESP_LOGW(SPI_TAG, "ERROR initializing");
		Initialized = false;
	}

    return Initialized;
}

void SPI::SetupPin(const gpio_num_t Pin) const noexcept
{
    gpio_reset_pin(Pin);
    gpio_set_direction(Pin, GPIO_MODE_OUTPUT);
    gpio_set_level(Pin, 0);
}

[[nodiscard]] spi_device_handle_t SPI::SetupDevice() const noexcept
{
    spi_device_handle_t Handle = nullptr;
    spi_device_interface_config_t InterfaceConfig = {};

    InterfaceConfig.mode = 0;
    InterfaceConfig.spics_io_num = Pins->CS;
    InterfaceConfig.clock_speed_hz = Config->CLOCK_SPEED_HZ;
    InterfaceConfig.queue_size = 1;

    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &InterfaceConfig, &Handle));
    return Handle;
}

[[nodiscard]] bool SPI::BuildConfig()
{
    BusConfig.mosi_io_num     = Pins->MOSI;
    BusConfig.miso_io_num     = Pins->MISO;
    BusConfig.sclk_io_num     = Pins->SCLK;
    BusConfig.quadwp_io_num   = -1;
    BusConfig.quadhd_io_num   = -1;
    BusConfig.max_transfer_sz = 0;
    BusConfig.flags           = 0;

    return ValidateConfig();
}

[[nodiscard]] bool SPI::ValidateConfig() const
{
    bool Valid = true;

    if (!BusConfig.mosi_io_num)
	{
        ESP_LOGW(SPI_TAG, "ERROR failed to provide MOSI pin");
        Valid = false;
	}
    else if (!BusConfig.miso_io_num)
	{
        ESP_LOGW(SPI_TAG, "ERROR failed to provide MISO pin");
        Valid = false;
	}
    else if (!BusConfig.sclk_io_num)
	{
        ESP_LOGW(SPI_TAG, "ERROR failed to provide SCLK pin");
		Valid = false;
	}

    return Valid;
}

void SPI::Start() const noexcept
{
    WriteByte(Commands.SET_DISPLAY_OFF ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_DISPLAY_CLOCK_FREQ ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SYS_CALL ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_MULTIPLEX_RATIO ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(0x3F ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_DISPLAY_OFFSET ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(ControlBytes.DUMMY_BYTE ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_DISPLAY_START_LINE ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_SEGMENT_REMAP_127 ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_COM_PINS_HARDWARE_CONFIG ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(0x12 ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_CONTRAST_CONTROL ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.DEFAULT_CONTRAST ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_PRECHARGE_PERIOD ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte((Commands.PHASE_TWO << 4 ) | Commands.PHASE_ONE ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_VCOM_DESELECT_LEVEL ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(0x40 ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.CHARGE_PUMP_SETTING ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.ENABLE_CHARGE_PUMP ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_MEMORY_ADDRESSING_MODE,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.PAGE_ADDRESSING_MODE,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_LOWER_COLUMN_START + 0x00 ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_HIGHER_COLUMN_START + 0x00 ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_PAGE_START | 0x00 ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.ENTIRE_DISPLAY_ON_WITH_CONTENT ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_NORMAL_DISPLAY ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.DEACTIVATE_SCROLL ,ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_DISPLAY_ON ,ControlBytes.COMMAND_MODE, Pins->DC);
}

[[gnu::cold]] bool SPI::Probe()
{
    ESP_LOGI(SPI_TAG, "Probe : Begin");
    spi_transaction_t Transaction = {};
    Transaction.flags = SPI_TRANS_USE_TXDATA;
    Transaction.length = Config->BIT_SIZE;
    Transaction.tx_data[0] = Commands.SET_DISPLAY_ON;

    ESP_LOGI(SPI_TAG, "Probe : Transmitting");
    gpio_set_level((gpio_num_t)Pins->DC, 0);
    esp_err_t Response = spi_device_transmit(DeviceHandle, &Transaction);
    ESP_LOGI(SPI_TAG, "Probe : Response : %s", esp_err_to_name(Response));

    vTaskDelay(TIMEOUT);
    Transaction.flags = SPI_TRANS_USE_TXDATA;
    Transaction.length = Config->BIT_SIZE;
    Transaction.tx_data[0] = Commands.SET_DISPLAY_OFF;
    Response = spi_device_transmit(DeviceHandle, &Transaction);

   return Response == ESP_OK;
}

[[gnu::hot]] void SPI::Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Offset, const size_t Bytes, uint8_t* Data) const noexcept
{
    IndexGDDRAM(Segment, Page, Offset);
    WriteBuffer(Data, Bytes, ControlBytes.DATA_MODE, Pins->DC);
    ESP_LOGI(SPI_TAG,"Draw");
}

[[gnu::hot]] void SPI::Draw(const uint8_t SegmentLow, const uint8_t SegmentHigh, const uint8_t Page, const uint8_t Offset, const size_t Bytes, uint8_t* Data) const noexcept
{
    IndexGDDRAM(SegmentLow, SegmentHigh, Page, Offset);
    WriteBuffer(Data, Bytes, ControlBytes.DATA_MODE, Pins->DC);
    ESP_LOGI(SPI_TAG,"Draw");
}

[[gnu::hot]] void SPI::Scroll(const Direction Direction, const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept
{
    switch (Direction)
    {
        case Direction::HORIZONTAL:
            ScrollHorizontal(ScrollCommand);
            break;
        case Direction::VERTICAL:
            ScrollVertical(ScrollCommand, VerticalOffset);
            break;
        default:
            break;
    }
}

void SPI::ScrollHorizontal(const uint8_t ScrollCommand) const noexcept
{
    WriteByte(ScrollCommand, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(ControlBytes.DUMMY_BYTE, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(0x00, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(0x07, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(0x00, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(0xFE, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(Commands.ACTIVATE_SCROLL, ControlBytes.DATA_MODE, Pins->DC);
}

void SPI::ScrollVertical(const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept
{
    WriteByte(ScrollCommand,ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(ControlBytes.DUMMY_BYTE, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(0x00, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(0x07, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(0x00, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(VerticalOffset, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(Commands.SET_VERTICAL_SCROLL_AREA, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(ControlBytes.DUMMY_BYTE, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(Commands.SET_DISPLAY_START_LINE, ControlBytes.DATA_MODE, Pins->DC);
    WriteByte(Commands.ACTIVATE_SCROLL, ControlBytes.DATA_MODE, Pins->DC);
}

void SPI::Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept
{
    uint8_t* DummyBuffer = nullptr;
    Draw(Segment, Page, Offset, 0, DummyBuffer);
}

void SPI::Flush() const noexcept
{
    static uint8_t DummyBuffer[8] = { 0b00000000 };
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
        Draw(Column, Row, 0, Size, &DummyBuffer[0]);
    }
}

bool SPI::WriteByte(const int Data, const uint8_t CommandByte, const uint8_t Pin) const noexcept
{
    spi_transaction_t Transaction = {};

    uint32_t MyData = static_cast<uint8_t>(Data);
    Transaction.length = Config->BIT_SIZE;
    Transaction.flags = SPI_TRANS_USE_TXDATA;
    Transaction.tx_data[0] = MyData;
    Mode(CommandByte, Pin);
    spi_device_transmit(DeviceHandle, &Transaction);

    return true;
}

bool SPI::WriteBuffer(const uint8_t* Data, const size_t Bytes, const uint8_t CommandByte, const uint8_t Pin) const noexcept
{
    spi_transaction_t Transaction = {};
    Transaction.length = Bytes * Config->BIT_SIZE;
    Transaction.tx_buffer = Data;
    Mode(CommandByte, Pin);
    spi_device_transmit(DeviceHandle, &Transaction);
    return true;
}

void SPI::Mode(const uint8_t CommandByte, const uint8_t Pin) const noexcept
{
    gpio_set_level((gpio_num_t)Pin, CommandByte);
}

void SPI::IndexGDDRAM(const uint8_t Segment, const uint8_t Page, const uint8_t Offset) const noexcept
{
    const uint8_t SegmentLow = OffsetLow(Segment, Offset);
    const uint8_t SegmentHigh = OffsetHigh(Segment, Offset);
    WriteByte(Commands.SET_LOWER_COLUMN_START + SegmentLow , ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_HIGHER_COLUMN_START + SegmentHigh , ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_PAGE_START | Page , ControlBytes.COMMAND_MODE, Pins->DC);
}

void SPI::IndexGDDRAM(const uint8_t SegmentLow, const uint8_t SegmentHigh, const uint8_t Page, const uint8_t Offset) const noexcept
{
    const uint8_t OffsetSegmentLow = OffsetLow(SegmentLow, Offset);
    const uint8_t OffsetSegmentHigh = OffsetHigh(SegmentHigh, Offset);
    WriteByte(Commands.SET_LOWER_COLUMN_START + OffsetSegmentLow , ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_HIGHER_COLUMN_START + OffsetSegmentHigh , ControlBytes.COMMAND_MODE, Pins->DC);
    WriteByte(Commands.SET_PAGE_START | Page , ControlBytes.COMMAND_MODE, Pins->DC);
}