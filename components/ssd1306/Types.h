#ifndef SSD1306_TYPES_H
#define SSD1306_TYPES_H

enum class Directions
{
    HORIZONTAL=0,
    VERTICAL,
    STOP
};

enum class Protocols
{
    SPI_BUS=0,
    I2C_BUS,
    UART_BUS
};

enum class Statuses
{
    NONE = 0,
    OK = 1,
    ERROR = 2
};

#endif