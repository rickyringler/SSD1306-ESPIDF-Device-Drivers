#include "include.h"

extern "C" void app_main(void)
{
	/* 1 : Create 8-bit binary representation of something */
    uint8_t SmileyFace[8] =
    {
        0b00111100, // PAGE 0
        0b01000010, // PAGE 1
        0b10100101, // PAGE 2 (eyes)
        0b10000001, // PAGE 3
        0b10100101, // PAGE 4 (eyes)
        0b10011001, // PAGE 5 (mouth)
        0b01000010, // PAGE 6
        0b00111100  // PAGE 7
    };

    /* 2 : Declare what we want. Override defaults if needed. */
    constexpr  uint8_t  ADDRESS            = 0xFF;
    constexpr  uint16_t WIDTH              = 64;
    constexpr  uint16_t HEIGHT             = 128;
    constexpr  int      BIT_SIZE           = 8;
    constexpr  int      CLOCK_SPEED_HZ     = 400000;
    constexpr  int      I2C_MODE_MASTER    = 1;
    constexpr  int      GPIO_PULLUP_ENABLE = 1;

    /* 3 : Initialize the config, pins, and device. */
    const SSD1306Configuration DisplayConfig(CLOCK_SPEED_HZ, I2C_MODE_MASTER, GPIO_PULLUP_ENABLE, BIT_SIZE, WIDTH, HEIGHT, ADDRESS);
    const SSD1306Pins DisplayPins(GPIO_NUM_5, GPIO_NUM_18, GPIO_NUM_33, GPIO_NUM_27);
    const SSD1306 Display(DisplayConfig, DisplayPins, Protocol(I2C_BUS));

    if (Display.GetStatus() != Status::OK)
    {
        exit(0);
    }

    /* 4 : Enter draw loop. Flush out every segment of each page from a previous run. */
    int Column = 0;
    int Columns = DisplayConfig.HEIGHT + 1;
    Display.Flush();
    while (true)
    {
        for (int Row = 0; Column < Columns; Row++)
        {
            static constexpr size_t Size = 8;
            if ((Row >> 3) << 3 == Row)
            {
                Row = 0;
                Column++;
            }
            Display.Draw(Column, Row, 0, Size, &SmileyFace[0]);
        }
    }

    vTaskDelay(TIMEOUT);
}