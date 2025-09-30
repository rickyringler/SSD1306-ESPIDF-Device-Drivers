#include "I2C.h"
#include <esp_log.h>


I2C::I2C(uint8_t Address, const SSD1306Configuration& Configuration)
    : Address(Address), DeviceConfig(Configuration)
{
    Configure();
}

bool I2C::Configure() noexcept
{
    const esp_err_t Error = BuildConfig();
    if (Error != ESP_OK)
    ESP_LOGW(TAG, "ERROR building Config");

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &BusConfig));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

    return true;
}

bool I2C::Start() const noexcept
{
    const i2c_cmd_handle_t Command = i2c_cmd_link_create();
    i2c_master_start(Command);
    i2c_master_write_byte(Command, (Address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(Command, ControlBytes.COMMAND_MODE, true);
    i2c_master_write_byte(Command, Commands.SET_MULTIPLEX_RATIO, true);
    i2c_master_write_byte(Command, Commands.SET_LOWER_COLUMN_START, true);
    i2c_master_write_byte(Command, Commands.SET_DISPLAY_OFFSET, true);
    i2c_master_write_byte(Command, Commands.SET_HIGHER_COLUMN_START, true);
    i2c_master_write_byte(Command, Commands.SET_DISPLAY_START_LINE, true);
    i2c_master_write_byte(Command, Commands.SET_SEGMENT_REMAP_127, true);
    i2c_master_write_byte(Command, Commands.SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL, true);
    i2c_master_write_byte(Command, Commands.SET_DISPLAY_CLOCK_FREQ, true);
    i2c_master_write_byte(Command, Commands.SYS_CALL, true);
    i2c_master_write_byte(Command, Commands.SET_COM_PINS_HARDWARE_CONFIG, true);
    i2c_master_write_byte(Command, Commands.SET_CONTRAST_CONTROL, true);
    i2c_master_write_byte(Command, Commands.SET_DISPLAY_ON, true);
    i2c_master_write_byte(Command, Commands.SET_NORMAL_DISPLAY, true);
    i2c_master_write_byte(Command, Commands.SET_DISPLAY_CLOCK_FREQ, true);
    i2c_master_write_byte(Command, Commands.CHARGE_PUMP_SETTING, true);
    i2c_master_write_byte(Command, Commands.SET_DISPLAY_ON, true);

    i2c_master_stop(Command);
    i2c_master_cmd_begin(I2C_NUM_1, Command, 1000/portTICK_PERIOD_MS);
    return true;
}
[[gnu::cold]] void I2C::BusAddressScan() const
{
    i2c_port_t Port = I2C_NUM_0;
    ESP_LOGI(TAG, "Scanning I2C port %d  : ", Port);
    int Found = 0;
    for (uint8_t addr = 1; addr < 0x7F; ++addr) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);
        esp_err_t r = i2c_master_cmd_begin(Port, cmd, pdMS_TO_TICKS(50));
        i2c_cmd_link_delete(cmd);
        if (r == ESP_OK) {
            ESP_LOGI(TAG, "I2C device found at 0x%02X", addr);
            ++Found;
        }
        vTaskDelay(pdMS_TO_TICKS(2));
    }
    ESP_LOGI(TAG, "Scan complete: %d device(s) found.", Found);
}

[[gnu::hot]] void I2C::Mode(const uint8_t Mode, i2c_cmd_handle_t& CommandHandle) const noexcept
{
    i2c_master_write_byte(CommandHandle, Mode, true);
}

[[nodiscard]] esp_err_t I2C::BuildConfig() noexcept
{
    BusConfig.mode             = I2C_MODE_MASTER;
    BusConfig.sda_io_num       = Pins.SDA;
    BusConfig.scl_io_num       = Pins.SCL;
    BusConfig.sda_pullup_en    = GPIO_PULLUP_ENABLE;
    BusConfig.scl_pullup_en    = GPIO_PULLUP_ENABLE;
    BusConfig.master.clk_speed = DeviceConfig.CLOCK_SPEED_HZ;

    return ValidateConfig() == false ? ESP_FAIL : ESP_OK;
}

[[nodiscard]] bool I2C::ValidateConfig() const noexcept
{
    const uint8_t ValuesToValidate = 6;
    uint8_t Validations = ValuesToValidate;

    if (BusConfig.mode == DeviceConfig.I2C_MODE_MASTER)
        Validations--;
    if (BusConfig.sda_io_num == Pins.SDA)
        Validations--;
    if (BusConfig.scl_io_num == Pins.SCL)
        Validations--;
    if (BusConfig.sda_pullup_en == DeviceConfig.GPIO_PULLUP_ENABLE)
        Validations--;
    if (BusConfig.scl_pullup_en == DeviceConfig.GPIO_PULLUP_ENABLE)
        Validations--;
    if (BusConfig.master.clk_speed == DeviceConfig.CLOCK_SPEED_HZ)
        Validations--;

    return Validations == ValuesToValidate;
}

[[gnu::hot]] void I2C::Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Width, uint8_t Offset, uint8_t Data) const noexcept
{
    IndexGDDRAM(Segment, Page, Offset);
    i2c_cmd_handle_t Command = i2c_cmd_link_create();
    i2c_master_start(Command);
    i2c_master_write_byte(Command, Address << I2C_MASTER_WRITE, true);
    Mode(ControlBytes.DATA_MODE, Command);
    i2c_master_write(Command, &Data, Width, true);
    i2c_master_stop(Command);
    i2c_master_cmd_begin(I2C_NUM_1, Command, 1000/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(Command);
}

[[gnu::hot]] void I2C::Scroll(const Direction Direction, const uint8_t ScrollCommand, const uint8_t VerticalOffset) const noexcept
{
    i2c_cmd_handle_t Command = i2c_cmd_link_create();
    i2c_master_start(Command);
    i2c_master_write_byte(Command, (Address << 1) | I2C_MASTER_WRITE, true);
    Mode(ControlBytes.COMMAND_MODE, Command);

    switch (Direction)
    {
        case Direction::HORIZONTAL:
            ScrollHorizontal(ScrollCommand, Command);
            break;
        case Direction::VERTICAL:
            ScrollVertical(ScrollCommand, Command, VerticalOffset);
            break;
        default:
            i2c_master_write_byte(Command, Commands.DEACTIVATE_SCROLL, true);
            break;
    }
    i2c_master_stop(Command);
    i2c_master_cmd_begin(I2C_NUM_1, Command, 1000/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(Command);
}

[[gnu::hot]]  void I2C::ScrollHorizontal(const uint8_t CommandByte, i2c_cmd_handle_t& Command) const noexcept
{
    i2c_master_write_byte(Command, CommandByte, true);
    i2c_master_write_byte(Command, ControlBytes.DUMMY_BYTE, true);
    i2c_master_write_byte(Command, 0x00, true);
    i2c_master_write_byte(Command, 0x07, true);
    i2c_master_write_byte(Command, 0x07, true);
    i2c_master_write_byte(Command, 0x00, true);
    i2c_master_write_byte(Command, 0xFE, true);
    i2c_master_write_byte(Command, Commands.ACTIVATE_SCROLL, true);
}

[[gnu::hot]]  void I2C::ScrollVertical(const uint8_t CommandByte, i2c_cmd_handle_t& Command, const uint8_t VerticalOffset) const noexcept
{
    i2c_master_write_byte(Command, CommandByte, true);
    i2c_master_write_byte(Command, ControlBytes.DUMMY_BYTE, true);
    i2c_master_write_byte(Command, 0x00, true);
    i2c_master_write_byte(Command, 0x07, true);
    i2c_master_write_byte(Command, 0x00, true);
    i2c_master_write_byte(Command, VerticalOffset, true);
    i2c_master_write_byte(Command, Commands.SET_VERTICAL_SCROLL_AREA, true);
    i2c_master_write_byte(Command, ControlBytes.DUMMY_BYTE, true);
    i2c_master_write_byte(Command, Commands.SET_DISPLAY_START_LINE, true);
    i2c_master_write_byte(Command, Commands.ACTIVATE_SCROLL, true);
}

[[gnu::hot]] void I2C::Clear(const uint8_t Segment, const uint8_t Page, const uint8_t Width, const uint8_t Offset) const noexcept
{
    uint8_t DummyBuffer[16] = {};
    Draw(Segment, Page, Width, Offset, *DummyBuffer);
}

[[gnu::hot]] void I2C::Flush() const noexcept
{
    uint8_t DummyBuffer[16] = {};
    for (uint8_t Page=0; Page < DeviceConfig.PAGES; Page++)
    {
        Draw(Page, Page, Page, 0, *DummyBuffer);
    }
}

[[gnu::hot]] void I2C::IndexGDDRAM(const uint8_t Segment, const uint8_t Page, uint8_t Offset) const noexcept
{
    const uint8_t SegmentLow = OffsetLow(Segment, Offset);
    const uint8_t SegmentHigh = OffsetHigh(Segment, Offset);
    i2c_cmd_handle_t Command = i2c_cmd_link_create();
    i2c_master_start(Command);
    i2c_master_write_byte(Command, Address << I2C_MASTER_WRITE, true);
    Mode(ControlBytes.COMMAND_MODE, Command);
    i2c_master_write_byte(Command, Commands.SET_LOWER_COLUMN_START + SegmentLow, true);
    i2c_master_write_byte(Command, Commands.SET_HIGHER_COLUMN_START + SegmentHigh, true);
    i2c_master_write_byte(Command, Commands.SET_PAGE_START | Page, true);
    i2c_master_stop(Command);
    i2c_master_cmd_begin(I2C_NUM_1, Command, 1000/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(Command);
}
[[nodiscard]] uint8_t I2C::OffsetLow(uint8_t Segment, uint8_t Offset) const noexcept
{
    const uint8_t OffsetLow = (Segment + Offset) & 0x0F;
    return OffsetLow;
}
[[nodiscard]] uint8_t I2C::OffsetHigh(uint8_t Segment, uint8_t Offset) const noexcept
{
    const uint8_t OffsetHigh = ((Segment + Offset) >> 4) & 0x0F;
    return OffsetHigh;
}