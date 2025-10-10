#ifndef SSD1306_3D_DEVICE_DRIVER_COMMANDS_H
#define SSD1306_3D_DEVICE_DRIVER_COMMANDS_H

struct SSD1306Commands
{
    static constexpr int SYS_CALL                                 = 0x80;
    static constexpr int SET_CONTRAST_CONTROL                     = 0x81;
        static constexpr int DEFAULT_CONTRAST                     = 0x7F;
    static constexpr int ENTIRE_DISPLAY_ON_WITH_CONTENT           = 0xA4;
    static constexpr int ENTIRE_DISPLAY_ON_WITHOUT_CONTENT        = 0xA5;
    static constexpr int SET_NORMAL_DISPLAY                       = 0xA6;
    static constexpr int SET_INVERSE_DISPLAY                      = 0xA7;
    static constexpr int SET_DISPLAY_OFF                          = 0xAE;
    static constexpr int SET_DISPLAY_ON                           = 0xAF;
    static constexpr int ACTIVATE_SCROLL                          = 0x2F;
    static constexpr int DEACTIVATE_SCROLL                        = 0x2E;
    static constexpr int CONTINUOUS_HORIZONTAL_SCROLL_SETUP_RIGHT = 0x26;
    static constexpr int CONTINUOUS_HORIZONTAL_SCROLL_SETUP_LEFT  = 0x27;
    static constexpr int CONTINUOUS_VERTICAL_SCROLL_SETUP_RIGHT   = 0x28;
    static constexpr int CONTINUOUS_VERTICAL_SCROLL_SETUP_LEFT    = 0x29;
    static constexpr int SET_VERTICAL_SCROLL_AREA                 = 0xA3;
    static constexpr int SET_LOWER_COLUMN_START                   = 0x00;
    static constexpr int SET_HIGHER_COLUMN_START                  = 0x10;
    static constexpr int SET_MEMORY_ADDRESSING_MODE               = 0x20;
        static constexpr int HORIZONTAL_ADDRESSING_MODE           = 0x00;
        static constexpr int VERTICAL_ADDRESSING_MODE             = 0x01;
        static constexpr int PAGE_ADDRESSING_MODE                 = 0x02;
    static constexpr int SET_COLUMN_ADDRESS                       = 0x21;
    static constexpr int SET_PAGE_ADDRESS                         = 0x22;
    static constexpr int SET_PAGE_START                           = 0xB0;
    static constexpr int SET_DISPLAY_START_LINE                   = 0x40;
    static constexpr int SET_SEGMENT_REMAP_0                      = 0xA0;
    static constexpr int SET_SEGMENT_REMAP_127                    = 0xA1;
    static constexpr int SET_MULTIPLEX_RATIO                      = 0xA8;
    static constexpr int SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL     = 0xC0;
    static constexpr int SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED   = 0xC8;
    static constexpr int SET_DISPLAY_OFFSET                       = 0xD3;
    static constexpr int SET_COM_PINS_HARDWARE_CONFIG             = 0xDA;
    static constexpr int SET_DISPLAY_CLOCK_FREQ                   = 0xD5;
    static constexpr int SET_PRECHARGE_PERIOD                     = 0xD9;
        static constexpr int PHASE_ONE                            = 0x01;
        static constexpr int PHASE_TWO                            = 0x0F;
    static constexpr int CHARGE_PUMP_SETTING                      = 0x8D;
        static constexpr int ENABLE_CHARGE_PUMP                   = 0x14;
        static constexpr int DISABLE_CHARGE_PUMP                  = 0x10;
    static constexpr int SET_VCOM_DESELECT_LEVEL                  = 0xDB;
    static constexpr int NOP                                      = 0xE3;
};

#endif