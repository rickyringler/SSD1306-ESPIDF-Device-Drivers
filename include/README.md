# SSD1306

## Datasheet:
https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf

## Specifications:
| Parameter | Description |
|------------|-------------|
| **Display Type** | Monochrome OLED |
| **Resolution** | 128 × 64 pixels |
| **Physical Size** | 0.96 inch (24.384 mm) |
| **Segments (Columns)** | 128 |
| **Pages (Rows)** | 8 |
| **Communication Protocols** | I²C or SPI |

## Instructions:
  1. Intiailize SSD1306Configuration
  2. Initialize SSD1306Pins
  3. Construct SSD1306
  4. Call public SSD1306 methods

 ## Example:

  ````cpp
#include "../include/include.h"

extern "C" void app_main(void)
{
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

    const uint8_t  ADDRESS            = 0xFF;
    const uint16_t WIDTH              = 64;
    const uint16_t HEIGHT             = 128;
    const int      BIT_SIZE           = 8;
    const int      CLOCK_SPEED_HZ     = 400000;
    const int      I2C_MODE_MASTER    = 1;
    const int      GPIO_PULLUP_ENABLE = 1;

    SSD1306Configuration MCUConfig(
        CLOCK_SPEED_HZ,
        I2C_MODE_MASTER,
        GPIO_PULLUP_ENABLE,
        BIT_SIZE,
        WIDTH,
        HEIGHT,
        ADDRESS
    );

    SSD1306Pins MCUPins(GPIO_NUM_5, GPIO_NUM_18, GPIO_NUM_33, GPIO_NUM_27);
    SSD1306 MCU(MCUConfig, MCUPins, Protocol(SPI_BUS));

    size_t Size = 8;

    int Column = 0;
    for (int Row = 0; Column < 129; Row++)
    {
        if ((Row >> 3) << 3 == Row)
        {
            Row = 0;
            Column++;
        }
        MCU.Draw(Column, Row, 0, Size, &SmileyFace[0]);
    }

    vTaskDelay(TIMEOUT);
}````
