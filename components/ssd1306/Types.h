#ifndef SSD1306_TYPES_H
#define SSD1306_TYPES_H

enum Direction
{
    HORIZONTAL=0,
    VERTICAL,
    STOP
};

enum Protocol
{
    SPI_BUS=0,
    I2C_BUS,
    UART_BUS
};

enum Status
{
    NONE = 0,
    OK = 1,
    ERROR = 2
};

#endif