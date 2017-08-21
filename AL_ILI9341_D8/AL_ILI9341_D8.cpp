/*
 * Copyright Anders Liu.
 *
 * For more information, visit https://github.com/anders-liu/arduino-libs/tree/master/AL_ILI9341_D8
 */

#include <avr/pgmspace.h>
#include "AL_ILI9341_D8.h"

#ifdef SUPPORT_DRAW_TEXT
#include "FontData.cpp"
#endif

#define CMD_NOP 0x00
#define CMD_SOFT_RESET 0x01
#define CMD_READ_STATUS 0x09
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

#define CMD_READ_ID4 0xD3

#define PF_16BIT 0x55
#define PF_18BIT 0x66

#define MA_MIRROR_X 0x80
#define MA_MIRROR_Y 0x40
#define MA_ROW_COL_EX 0x20
#define MA_V_ORDER 0x10
#define MA_RGB 0x08
#define MA_H_ORDER 0x04

const static uint8_t soMA[] PROGMEM =
    {
        // AL_SO_LANDSCAPE1 = 0
        MA_ROW_COL_EX | MA_RGB,

        // AL_SO_LANDSCAPE2 = 1
        MA_ROW_COL_EX | MA_RGB | MA_MIRROR_X | MA_MIRROR_Y,

        // AL_SO_PORTRAIT1 = 2
        MA_MIRROR_X | MA_RGB,

        // AL_SO_PORTRAIT2 = 3
        MA_MIRROR_Y | MA_RGB};

#ifdef RECOVER_PIN_BEFORE_TRANSFER
#define RECOVER_PIN               \
    {                             \
        pinMode(rsPin, OUTPUT);   \
        pinMode(csPin, OUTPUT);   \
        digitalWrite(csPin, LOW); \
    }
#else
#define RECOVER_PIN
#endif

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

#define GET_COLOR_2BYTE(color, b1, b2) \
    {                                  \
        uint16_t cv = color565(color); \
        b1 = cv >> 8;                  \
        b2 = cv;                       \
    }

void AL_ILI9341_D8::setup()
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

    RECOVER_PIN;
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
    WRITE_BYTE(pgm_read_byte_near(soMA + getOrientation()));

    BEGIN_CMD;
    WRITE_BYTE(CMD_SLEEP_OUT);
    WRITE_BYTE(CMD_DISP_ON);
}

uint32_t AL_ILI9341_D8::readStatus()
{
    uint8_t x;
    uint32_t res = 0;

    RECOVER_PIN;
    BEGIN_CMD;
    WRITE_BYTE(CMD_READ_STATUS);

    BEGIN_DATA_IN;
    READ_BYTE(x); // dummy
    READ_BYTE(x);
    res = x;
    res <<= 8;
    READ_BYTE(x);
    res |= x;
    res <<= 8;
    READ_BYTE(x);
    res |= x;
    res <<= 8;
    READ_BYTE(x);
    res |= x;

    return res;
}

uint32_t AL_ILI9341_D8::readID()
{
    uint8_t x;
    uint32_t res = 0;

    RECOVER_PIN;
    BEGIN_CMD;
    WRITE_BYTE(CMD_READ_ID4);

    BEGIN_DATA_IN;
    READ_BYTE(x); // dummy
    READ_BYTE(x);
    res = x;
    res <<= 8;
    READ_BYTE(x);
    res |= x;
    res <<= 8;
    READ_BYTE(x);
    res |= x;

    return res;
}

uint8_t AL_ILI9341_D8::readPowerMode()
{
    uint8_t x;

    RECOVER_PIN;
    BEGIN_CMD;
    WRITE_BYTE(CMD_READ_POWER_MODE);

    BEGIN_DATA_IN;
    READ_BYTE(x); // dummy
    READ_BYTE(x);

    return x;
}

void AL_ILI9341_D8::displayOff()
{
    RECOVER_PIN;
    BEGIN_CMD;
    WRITE_BYTE(CMD_DISP_OFF);
}

void AL_ILI9341_D8::displayOn()
{
    RECOVER_PIN;
    BEGIN_CMD;
    WRITE_BYTE(CMD_DISP_ON);
}

void AL_ILI9341_D8::sleepIn()
{
    RECOVER_PIN;
    BEGIN_CMD;
    WRITE_BYTE(CMD_SLEEP_IN);
}

void AL_ILI9341_D8::sleepOut()
{
    RECOVER_PIN;
    BEGIN_CMD;
    WRITE_BYTE(CMD_SLEEP_OUT);
}

void AL_ILI9341_D8::clear(AL_RgbColor color)
{
    displayOff();
    fillRect(0, 0, getWidth(), getHeight(), color);
    displayOn();
}

void AL_ILI9341_D8::changeOrientation(AL_ScreenOrientation o)
{
    RECOVER_PIN;

    BEGIN_CMD;
    WRITE_BYTE(CMD_MEMORY_ACCESS_CONTROL);

    BEGIN_DATA_OUT;
    WRITE_BYTE(pgm_read_byte_near(soMA + o));
}

void AL_ILI9341_D8::fillRect(
    uint16_t x, uint16_t y,
    uint16_t w, uint16_t h,
    AL_RgbColor color)
{
    RECOVER_PIN;

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
}

#ifdef SUPPORT_DRAW_TEXT

#ifdef USE_FONT_5X7

// 5x7 font saves char pattern turned 90 degree clockwise.
#define WRITE_CHAR_PATTERN(pi, fcb1, fcb2, bcb1, bcb2)             \
    {                                                              \
        uint8_t pattern[CHAR_PATTERN_BYTES];                       \
        for (uint8_t i = 0; i < CHAR_PATTERN_BYTES; i++)           \
        {                                                          \
            pattern[i] = pgm_read_byte_near(fontData + pi + i);    \
        }                                                          \
        uint8_t mask = 1;                                          \
        for (uint8_t ppi = 0; ppi < TXT_CHAR_H; ppi++, mask <<= 1) \
        {                                                          \
            for (uint8_t off = 0; off < CHAR_PATTERN_BYTES; off++) \
            {                                                      \
                if ((pattern[off] & mask) != 0)                    \
                {                                                  \
                    WRITE_BYTE(fcb1);                              \
                    WRITE_BYTE(fcb2);                              \
                }                                                  \
                else                                               \
                {                                                  \
                    WRITE_BYTE(bcb1);                              \
                    WRITE_BYTE(bcb2);                              \
                }                                                  \
            }                                                      \
        }                                                          \
    }

#else

// Other fonts, one byte wide, top to bottom.
#define WRITE_CHAR_PATTERN(pi, fcb1, fcb2, bcb1, bcb2)                 \
    {                                                                  \
        for (uint8_t i = 0; i < CHAR_PATTERN_BYTES; i++)               \
        {                                                              \
            uint8_t pb = pgm_read_byte_near(fontData + pi + i);        \
            uint8_t mask = 0x80;                                       \
            for (uint8_t ppi = 0; ppi < TXT_CHAR_W; ppi++, mask >>= 1) \
            {                                                          \
                if ((pb & mask) != 0)                                  \
                {                                                      \
                    WRITE_BYTE(fcb1);                                  \
                    WRITE_BYTE(fcb2);                                  \
                }                                                      \
                else                                                   \
                {                                                      \
                    WRITE_BYTE(bcb1);                                  \
                    WRITE_BYTE(bcb2);                                  \
                }                                                      \
            }                                                          \
        }                                                              \
    }

#endif // USE_FONT_5X7

void AL_ILI9341_D8::drawText(
    uint16_t x, uint16_t y,
    AL_RgbColor foreColor, AL_RgbColor backColor,
    const char *str, uint8_t maxLen)
{
    RECOVER_PIN;

    uint8_t fcb1, fcb2, bcb1, bcb2;
    GET_COLOR_2BYTE(foreColor, fcb1, fcb2);
    GET_COLOR_2BYTE(backColor, bcb1, bcb2);

    for (uint8_t i = 0; i < maxLen && str[i] != 0; i++, x += TXT_CHAR_W)
    {
        setUpdateArea(x, y, TXT_CHAR_W, TXT_CHAR_H);
        uint16_t pi = ((uint8_t)str[i]) * CHAR_PATTERN_BYTES;

        BEGIN_CMD;
        WRITE_BYTE(CMD_MEMORY_WRITE);

        BEGIN_DATA_OUT;
        WRITE_CHAR_PATTERN(pi, fcb1, fcb2, bcb1, bcb2);
    }
}
#endif // SUPPORT_DRAW_TEXT

uint16_t AL_ILI9341_D8::color565(AL_RgbColor color)
{
    return ((color.r & B11111000) << 8) | ((color.g & B11111100) << 3) | (color.b >> 3);
}

void AL_ILI9341_D8::setUpdateArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
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
