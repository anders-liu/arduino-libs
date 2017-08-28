//
// AL_ILI9341.cpp
//
// Implement graphic screen on AL_ILI9341 based TFT screen.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#include "AL_ILI9341.h"
#include "AL_Font.h"

//
// Commands
//
#define CMD_NOP 0x00
#define CMD_SOFT_RESET 0x01
#define CMD_READ_POWER_MODE 0x0A
#define CMD_SLEEP_IN 0x10
#define CMD_SLEEP_OUT 0x11
#define CMD_DISP_OFF 0x28
#define CMD_DISP_ON 0x29
#define CMD_COL_ADDR_SET 0x2A
#define CMD_PAGE_ADDR_SET 0x2B
#define CMD_MEMORY_WRITE 0x2C
#define CMD_MEMORY_ACCESS_CONTROL 0x36
#define CMD_PIXEL_FORMAT_SET 0x3A

#define PF_16BIT 0x55
#define PF_18BIT 0x66

#define MA_MIRROR_X 0x80
#define MA_MIRROR_Y 0x40
#define MA_ROW_COL_EX 0x20
#define MA_V_ORDER 0x10
#define MA_RGB 0x08
#define MA_H_ORDER 0x04

#define MA_SO_PORTRAIT1 (MA_MIRROR_Y | MA_RGB)
#define MA_SO_PORTRAIT2 (MA_MIRROR_X | MA_RGB)
#define MA_SO_LANDSCAPE1 (MA_ROW_COL_EX | MA_RGB | MA_MIRROR_X | MA_MIRROR_Y)
#define MA_SO_LANDSCAPE2 (MA_ROW_COL_EX | MA_RGB)

#define BEGIN_BUS                 \
    {                             \
        pinMode(rsPin, OUTPUT);   \
        pinMode(csPin, OUTPUT);   \
        digitalWrite(csPin, LOW); \
    }

#define END_BUS                    \
    {                              \
        digitalWrite(csPin, HIGH); \
    }

#define BEGIN_CMD                 \
    {                             \
        *pDdr = 0xFF;             \
        digitalWrite(rsPin, LOW); \
    }

#define BEGIN_DATA_OUT             \
    {                              \
        *pDdr = 0xFF;              \
        digitalWrite(rsPin, HIGH); \
    }

#define BEGIN_DATA_IN              \
    {                              \
        *pDdr = 0x00;              \
        digitalWrite(rsPin, HIGH); \
    }

#define WRITE_BYTE(b)              \
    {                              \
        *pPort = b;                \
        digitalWrite(wrPin, LOW);  \
        digitalWrite(wrPin, HIGH); \
    }

#define READ_BYTE(b)               \
    {                              \
        digitalWrite(rdPin, LOW);  \
        b = *pPin;                 \
        digitalWrite(rdPin, HIGH); \
    }

#define GET_COLOR_2BYTE(color, b1, b2)    \
    {                                     \
        uint16_t cv = color.toColor565(); \
        b1 = cv >> 8;                     \
        b2 = cv;                          \
    }

void AL_ILI9341::setup()
{
    pinMode(rstPin, OUTPUT);
    pinMode(csPin, OUTPUT);
    pinMode(rsPin, OUTPUT);
    pinMode(wrPin, OUTPUT);
    pinMode(rdPin, OUTPUT);
    digitalWrite(rstPin, LOW);
    digitalWrite(csPin, LOW);
    digitalWrite(rsPin, LOW);
    digitalWrite(wrPin, HIGH);
    digitalWrite(rdPin, HIGH);
    digitalWrite(rstPin, HIGH);

    BEGIN_BUS;
    BEGIN_CMD;
    WRITE_BYTE(CMD_SOFT_RESET);
    delay(50);
    WRITE_BYTE(CMD_DISP_OFF);
    WRITE_BYTE(CMD_PIXEL_FORMAT_SET);
    BEGIN_DATA_OUT;
    WRITE_BYTE(PF_16BIT);
    BEGIN_CMD;
    WRITE_BYTE(CMD_MEMORY_ACCESS_CONTROL);
    BEGIN_DATA_OUT;
    WRITE_BYTE(MA_SO_PORTRAIT1);
    BEGIN_CMD;
    WRITE_BYTE(CMD_SLEEP_OUT);
    delay(15);
    WRITE_BYTE(CMD_DISP_ON);
    END_BUS;
}

void AL_ILI9341::changeOrientation()
{
    BEGIN_BUS;
    BEGIN_CMD;
    WRITE_BYTE(CMD_MEMORY_ACCESS_CONTROL);
    BEGIN_DATA_OUT;
    switch (getOrientation())
    {
    case AL_SO_PORTRAIT1:
        WRITE_BYTE(MA_SO_PORTRAIT1);
        break;
    case AL_SO_PORTRAIT2:
        WRITE_BYTE(MA_SO_PORTRAIT2);
        break;
    case AL_SO_LANDSCAPE1:
        WRITE_BYTE(MA_SO_LANDSCAPE1);
        break;
    case AL_SO_LANDSCAPE2:
        WRITE_BYTE(MA_SO_LANDSCAPE2);
        break;
    }
    END_BUS;
}

void AL_ILI9341::fillRect(
    uint16_t x, uint16_t y,
    uint16_t w, uint16_t h,
    AL_RgbColor color)
{
    BEGIN_BUS;
    setUpdateArea(x, y, w, h);
    BEGIN_CMD;
    WRITE_BYTE(CMD_MEMORY_WRITE);

    uint8_t b1, b2;
    GET_COLOR_2BYTE(color, b1, b2);
    uint32_t count = (uint32_t)w * (uint32_t)h;
    BEGIN_DATA_OUT;
    while (count--)
    {
        WRITE_BYTE(b1);
        WRITE_BYTE(b2);
    }
    END_BUS;
}

void AL_ILI9341::drawText(
    uint16_t x, uint16_t y,
    AL_RgbColor foreColor, AL_RgbColor backColor,
    const char *str, uint8_t maxLen)
{
    BEGIN_BUS;

    uint8_t fcb1, fcb2, bcb1, bcb2;
    GET_COLOR_2BYTE(foreColor, fcb1, fcb2);
    GET_COLOR_2BYTE(backColor, bcb1, bcb2);

    for (uint8_t i = 0; i < maxLen && str[i] != 0; i++, x += AL_FONT_CHAR_W)
    {
        setUpdateArea(x, y, AL_FONT_CHAR_W, AL_FONT_CHAR_H);
        uint16_t pi = ((uint8_t)str[i]) * AL_FONT_BYTES_PER_CHAR;

        BEGIN_CMD;
        WRITE_BYTE(CMD_MEMORY_WRITE);

        BEGIN_DATA_OUT;
        for (uint8_t i = 0; i < AL_FONT_BYTES_PER_CHAR; i++)
        {
            uint8_t pb = pgm_read_byte_near(AL_FontData + pi + i);
            uint8_t mask = 0x80;
            for (uint8_t ppi = 0; ppi < AL_FONT_CHAR_W; ppi++, mask >>= 1)
            {
                if ((pb & mask) != 0)
                {
                    WRITE_BYTE(fcb1);
                    WRITE_BYTE(fcb2);
                }
                else
                {
                    WRITE_BYTE(bcb1);
                    WRITE_BYTE(bcb2);
                }
            }
        }
    }
    END_BUS;
}

void AL_ILI9341::drawText(
    uint16_t x, uint16_t y,
    AL_RgbColor foreColor, AL_RgbColor backColor,
    uint8_t scale, const char *str, uint8_t maxLen)
{
    BEGIN_BUS;

    uint8_t fcb1, fcb2, bcb1, bcb2;
    GET_COLOR_2BYTE(foreColor, fcb1, fcb2);
    GET_COLOR_2BYTE(backColor, bcb1, bcb2);

    uint8_t pixelWidth = AL_FONT_CHAR_W * scale;
    uint8_t pixelHeight = AL_FONT_CHAR_H * scale;
    for (uint8_t i = 0; i < maxLen && str[i] != 0; i++, x += pixelWidth)
    {
        setUpdateArea(x, y, pixelWidth, pixelHeight);
        uint16_t pi = ((uint8_t)str[i]) * AL_FONT_BYTES_PER_CHAR;

        BEGIN_CMD;
        WRITE_BYTE(CMD_MEMORY_WRITE);

        BEGIN_DATA_OUT;
        for (uint8_t i = 0; i < AL_FONT_BYTES_PER_CHAR; i++)
        {
            // For each line, draw 'scale' times.
            for (uint8_t sl = 0; sl < scale; sl++)
            {
                uint8_t pb = pgm_read_byte_near(AL_FontData + pi + i);
                uint8_t mask = 0x80;
                for (uint8_t ppi = 0; ppi < AL_FONT_CHAR_W; ppi++, mask >>= 1)
                {
                    // For each pixel, draw 'scale' times.
                    if ((pb & mask) != 0)
                    {
                        for (uint8_t sp = 0; sp < scale; sp++)
                        {
                            WRITE_BYTE(fcb1);
                            WRITE_BYTE(fcb2);
                        }
                    }
                    else
                    {
                        for (uint8_t sp = 0; sp < scale; sp++)
                        {
                            WRITE_BYTE(bcb1);
                            WRITE_BYTE(bcb2);
                        }
                    }
                }
            }
        }
    }
    END_BUS;
}

void AL_ILI9341::setUpdateArea(
    uint16_t x, uint16_t y,
    uint16_t w, uint16_t h)
{
    BEGIN_CMD;
    WRITE_BYTE(CMD_COL_ADDR_SET);

    BEGIN_DATA_OUT;
    WRITE_BYTE((uint8_t)(x >> 8));
    WRITE_BYTE((uint8_t)x);
    x = x + w - 1;
    WRITE_BYTE((uint8_t)(x >> 8));
    WRITE_BYTE((uint8_t)x);

    BEGIN_CMD;
    WRITE_BYTE(CMD_PAGE_ADDR_SET);

    BEGIN_DATA_OUT;
    WRITE_BYTE((uint8_t)(y >> 8));
    WRITE_BYTE((uint8_t)y);
    y = y + h - 1;
    WRITE_BYTE((uint8_t)(y >> 8));
    WRITE_BYTE((uint8_t)y);
}
