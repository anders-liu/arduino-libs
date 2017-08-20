/*
 * Copyright Anders Liu.
 *
 * For more information, visit https://github.com/anders-liu/arduino-libs/tree/master/AL_ILI9341_D8
 */

#include "AL_ILI9341_D8.h"

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
    delay(1);
    digitalWrite(rstPin, HIGH);
    delay(200);

    RECOVER_PIN;
    BEGIN_CMD;
    WRITE_BYTE(0);
    WRITE_BYTE(0);
    WRITE_BYTE(0);
    WRITE_BYTE(0);
    delay(200);

    WRITE_BYTE(CMD_SOFT_RESET);
    delay(50);
    WRITE_BYTE(CMD_DISP_OFF);

    WRITE_BYTE(CMD_PIXEL_FORMAT_SET);
    BEGIN_DATA_OUT;
    WRITE_BYTE(PF_16BIT);

    BEGIN_CMD;
    WRITE_BYTE(CMD_MEMORY_ACCESS_CONTROL);
    BEGIN_DATA_OUT;
    WRITE_BYTE(0xE8);

    BEGIN_CMD;
    WRITE_BYTE(CMD_SLEEP_OUT);
    delay(150);
    WRITE_BYTE(CMD_DISP_ON);
    delay(500);
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

void AL_ILI9341_D8::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, RgbColor color)
{
    RECOVER_PIN;

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

    BEGIN_CMD;
    WRITE_BYTE(CMD_MEMORY_WRITE);

    uint16_t cv = color565(color);
    uint8_t b1 = cv >> 8;
    uint8_t b2 = cv;
    uint32_t count = (uint32_t)w * (uint32_t)h;
    BEGIN_DATA_OUT;
    while (count--)
    {
        WRITE_BYTE(b1);
        WRITE_BYTE(b2);
    }
}

uint16_t AL_ILI9341_D8::color565(RgbColor color)
{
    return ((color.r & B11111000) << 8) | ((color.g & B11111100) << 3) | (color.b >> 3);
}