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

## Protocols:
Both SPI and I2C are supported, but SPI is much more efficient here. SSD1306 devices may not always support I2C depending on the vendor.

The protocol selected in the ```Protocol``` field for the SSD1306 constructor will determine which protocol is used by the program.
