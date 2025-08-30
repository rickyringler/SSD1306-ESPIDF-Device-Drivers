#include "I2C.h"
#include <esp_log.h>

I2C::I2C(uint8_t Address, SSD1306ControlBytes ControlBytes, SSD1306Commands Commands, SSD1306Pins Pins, SSD1306Configuration DeviceConfig)
    : Address(Address), ControlBytes(ControlBytes), Commands(Commands), Pins(Pins), DeviceConfig(DeviceConfig)
{
    Configure();
}

bool I2C::Configure() noexcept
{
    const esp_err_t Error = BuildConfig();
    if (Error == ESP_OK)
    ESP_LOGW(TAG, "ERROR building Config");

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_1, &BusConfig));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_1, I2C_MODE_MASTER, 0, 0, 0));

    return true;
}

bool I2C::Start() const noexcept
{
    const i2c_cmd_handle_t Command = i2c_cmd_link_create();
    i2c_master_start(Command);
    i2c_master_write_byte(Command, (Address << 1) | I2C_MASTER_WRITE, true);
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
    i2c_master_cmd_begin(I2C_NUM_1, Command, 1000/portTICK_RATE_MS);
    return true;
}
void I2C::Mode(const uint8_t Mode, i2c_cmd_handle_t& CommandHandle) const noexcept
{
    i2c_master_write_byte(CommandHandle, Mode, true);
}

[[nodiscard]] esp_err_t I2C::BuildConfig() noexcept
{
    BusConfig.mode             = I2C_MODE_MASTER;
    BusConfig.sda_io_num       = Pins.SDA_PIN;
    BusConfig.scl_io_num       = Pins.SCL_PIN;
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
    if (BusConfig.sda_io_num == Pins.SDA_PIN)
        Validations--;
    if (BusConfig.scl_io_num == Pins.SCL_PIN)
        Validations--;
    if (BusConfig.sda_pullup_en == DeviceConfig.GPIO_PULLUP_ENABLE)
        Validations--;
    if (BusConfig.scl_pullup_en == DeviceConfig.GPIO_PULLUP_ENABLE)
        Validations--;
    if (BusConfig.master.clk_speed == DeviceConfig.CLOCK_SPEED_HZ)
        Validations--;

    return Validations == ValuesToValidate;
}

[[gnu::hot]] void I2C::Draw(const uint8_t Segment, const uint8_t Page, const uint8_t Width, uint8_t Offset, uint8_t& Data) const noexcept
{
    IndexGDDRAM(Segment, Page, Offset, Data);
    i2c_cmd_handle_t Command = i2c_cmd_link_create();
    i2c_master_start(Command);
    Mode(ControlBytes.COMMAND_MODE, Command);
    i2c_master_write_byte(Command, Address << I2C_MASTER_WRITE, true);
    Mode(ControlBytes.DATA_MODE, Command);
    i2c_master_write(Command, &Data, Width, true);
    i2c_master_stop(Command);
    i2c_master_cmd_begin(I2C_NUM_1, Command, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(Command);

}
[[gnu::hot]] void I2C::IndexGDDRAM(const uint8_t Segment, const uint8_t Page, uint8_t Offset, uint8_t& Data) const noexcept
{
    uint8_t SegmentLow = OffsetLow(Segment, Offset);
    uint8_t SegmentHigh = OffsetHigh(Segment, Offset);
    i2c_cmd_handle_t Command = i2c_cmd_link_create();
    i2c_master_start(Command);
    Mode(ControlBytes.COMMAND_MODE, Command);
    i2c_master_write_byte(Command, Address << I2C_MASTER_WRITE, true);
    Mode(ControlBytes.DATA_MODE, Command);
    i2c_master_write_byte(Command, Commands.SET_LOWER_COLUMN_START + SegmentLow, true);
    i2c_master_write_byte(Command, Commands.SET_HIGHER_COLUMN_START + SegmentHigh, true);
    i2c_master_write_byte(Command, Commands.SET_PAGE_START | Page, true);
    i2c_master_stop(Command);
    i2c_master_cmd_begin(I2C_NUM_1, Command, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(Command);
}
[[nodiscard]] uint8_t I2C::OffsetLow(uint8_t Segment, uint8_t Offset) const noexcept
{
    uint8_t OffsetLow = (Segment + Offset) & 0x0F;
    return OffsetLow;
}
[[nodiscard]] uint8_t I2C::OffsetHigh(uint8_t Segment, uint8_t Offset) const noexcept
{
    uint8_t OffsetHigh = ((Segment + Offset) >> 4) & 0x0F;
    return OffsetHigh;
}