//
// AL_TCS3472.cpp
//
// Provides functions for TCS3472 color sensor.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#include <Wire.h>
#include "AL_TCS3472.h"

//
// Calibration data
//

// Refer to https://github.com/adafruit/Adafruit_TCS34725/blob/12be4c6937df5f09b04a6540e1b3c00ce5fe93ac/examples/colorview/colorview.ino#L37
// but pre-calculated.
static const uint8_t gammaTable[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0A, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0C, 0x0C, 0x0D, 0x0D,
    0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x10, 0x10, 0x11, 0x11, 0x12, 0x12, 0x13, 0x13, 0x14, 0x15, 0x15,
    0x16, 0x16, 0x17, 0x17, 0x18, 0x19, 0x19, 0x1A, 0x1B, 0x1B, 0x1C, 0x1D, 0x1D, 0x1E, 0x1F, 0x1F,
    0x20, 0x21, 0x22, 0x22, 0x23, 0x24, 0x25, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2A, 0x2B, 0x2C,
    0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3B, 0x3C,
    0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4D, 0x4E,
    0x4F, 0x50, 0x52, 0x53, 0x54, 0x55, 0x57, 0x58, 0x59, 0x5B, 0x5C, 0x5D, 0x5F, 0x60, 0x62, 0x63,
    0x64, 0x66, 0x67, 0x69, 0x6A, 0x6C, 0x6D, 0x6F, 0x70, 0x72, 0x73, 0x75, 0x77, 0x78, 0x7A, 0x7B,
    0x7D, 0x7F, 0x80, 0x82, 0x84, 0x85, 0x87, 0x89, 0x8A, 0x8C, 0x8E, 0x90, 0x91, 0x93, 0x95, 0x97,
    0x99, 0x9B, 0x9C, 0x9E, 0xA0, 0xA2, 0xA4, 0xA6, 0xA8, 0xAA, 0xAC, 0xAE, 0xB0, 0xB2, 0xB4, 0xB6,
    0xB8, 0xBA, 0xBC, 0xBE, 0xC0, 0xC2, 0xC5, 0xC7, 0xC9, 0xCB, 0xCD, 0xCF, 0xD2, 0xD4, 0xD6, 0xD8,
    0xDB, 0xDD, 0xDF, 0xE2, 0xE4, 0xE6, 0xE9, 0xEB, 0xED, 0xF0, 0xF2, 0xF5, 0xF7, 0xFA, 0xFC, 0xFF};

//
// Commands
//

#define REG_ENABLE ((uint8_t)0xA0)
#define REG_ATIME ((uint8_t)0xA1)
#define REG_WTIME ((uint8_t)0xA3)
#define REG_AILTL ((uint8_t)0xA4)
#define REG_AILTH ((uint8_t)0xA5)
#define REG_AIHTL ((uint8_t)0xA6)
#define REG_AIHTH ((uint8_t)0xA7)
#define REG_PERS ((uint8_t)0xAC)
#define REG_CONFIG ((uint8_t)0xAD)
#define REG_CONTROL ((uint8_t)0xAF)
#define REG_ID ((uint8_t)0xB2)
#define REG_STATUS ((uint8_t)0xB3)
#define REG_CDATAL ((uint8_t)0xB4)
#define REG_CDATAH ((uint8_t)0xB5)
#define REG_RDATAL ((uint8_t)0xB6)
#define REG_RDATAH ((uint8_t)0xB7)
#define REG_GDATAL ((uint8_t)0xB8)
#define REG_GDATAH ((uint8_t)0xB9)
#define REG_BDATAL ((uint8_t)0xBA)
#define REG_BDATAH ((uint8_t)0xBB)

#define RC_ENABLE_AIEN ((uint8_t)B00010000)
#define RC_ENABLE_WEN ((uint8_t)B00001000)
#define RC_ENABLE_AEN ((uint8_t)B00000010)
#define RC_ENABLE_PON ((uint8_t)B00000001)

#define RC_CONFIG_WLONG ((uint8_t)B00000010)

#define RC_CONTROL_GAIN_1X ((uint8_t)0)
#define RC_CONTROL_GAIN_4X ((uint8_t)B00000001)
#define RC_CONTROL_GAIN_16X ((uint8_t)B00000010)
#define RC_CONTROL_GAIN_60X ((uint8_t)B00000011)

#define RC_STATUS_AINT ((uint8_t)B00010000)
#define RC_STATUS_AVALID ((uint8_t)B00000001)

#define DEFAULT_ATIME ((uint8_t)0xC0) /* 154ms */

void AL_TCS3472::setup()
{
    pinMode(ledPin, OUTPUT);

    Wire.beginTransmission(addr);
    // Set enable but power off.
    Wire.write(REG_ENABLE);
    Wire.write(RC_ENABLE_AEN);
    // Set default ATIME.
    Wire.write(DEFAULT_ATIME);
    Wire.endTransmission();

    // Set gain.
    Wire.beginTransmission(addr);
    Wire.write(REG_CONTROL);
    Wire.write(RC_CONTROL_GAIN_16X);
    Wire.endTransmission();
}

void AL_TCS3472::powerOn(bool ledOn)
{
    digitalWrite(ledPin, ledOn);

    Wire.beginTransmission(addr);
    Wire.write(REG_ENABLE);
    Wire.write(RC_ENABLE_AEN | RC_ENABLE_PON);
    Wire.endTransmission();
}

void AL_TCS3472::powerOff()
{
    Wire.beginTransmission(addr);
    Wire.write(REG_ENABLE);
    Wire.write(RC_ENABLE_AEN & ~RC_ENABLE_PON);
    Wire.endTransmission();

    digitalWrite(ledPin, LOW);
}

#define CALCULATE_CHANNEL_BASE_ON_CLEAR(res, value, clear) \
    {                                                      \
        if (clear == 0)                                    \
        {                                                  \
            res = 0;                                       \
        }                                                  \
        else if (value >= clear)                           \
        {                                                  \
            res = 0xFF;                                    \
        }                                                  \
        else                                               \
        {                                                  \
            float temp = value;                            \
            temp = temp * 256 / clear;                     \
            res = (uint8_t)temp;                           \
        }                                                  \
    }

AL_RgbColor AL_TCS3472::read()
{
    // Read raw data.
    Wire.beginTransmission(addr);
    Wire.write(REG_CDATAL);
    Wire.endTransmission();
    delayMicroseconds(100);

    uint8_t buffer[8];
    Wire.requestFrom((uint8_t)addr, (uint8_t)8);
    for (uint8_t i = 0; i < 8; i++)
    {
        while (!Wire.available())
            ;
        buffer[i] = Wire.read();
    }
    Wire.endTransmission();

    // Calculate color.
    uint16_t clear = (buffer[1] << 8) | buffer[0];
    uint16_t value;
    uint8_t r, g, b;

    value = (buffer[3] << 8) | buffer[2];
    CALCULATE_CHANNEL_BASE_ON_CLEAR(r, value, clear);
    value = (buffer[5] << 8) | buffer[4];
    CALCULATE_CHANNEL_BASE_ON_CLEAR(g, value, clear);
    value = (buffer[7] << 8) | buffer[6];
    CALCULATE_CHANNEL_BASE_ON_CLEAR(b, value, clear);

#if 1
    // Gamma correction
    r = pgm_read_byte_near(gammaTable + r);
    g = pgm_read_byte_near(gammaTable + g);
    b = pgm_read_byte_near(gammaTable + b);
#endif

    return AL_RgbColor(r, g, b);
}
