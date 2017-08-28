//
// AL_PCA9685.cpp
//
// Provides functions for PCA9685 16-channel PWM controller.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#include <Wire.h>
#include "AL_PCA9685.h"

#define REG_MODE1 ((uint8_t)0x00)
#define REG_MODE2 ((uint8_t)0x01)
#define REG_SUBADR1 ((uint8_t)0x02)
#define REG_SUBADR2 ((uint8_t)0x03)
#define REG_SUBADR3 ((uint8_t)0x04)
#define REG_ALLCALLADR ((uint8_t)0x05)
#define REG_LED0_ON_L ((uint8_t)0x06)
#define REG_LED0_ON_H ((uint8_t)0x07)
#define REG_LED0_OFF_L ((uint8_t)0x08)
#define REG_LED0_OFF_H ((uint8_t)0x09)
#define REG_LED1_ON_L ((uint8_t)0x0A)
#define REG_LED1_ON_H ((uint8_t)0x0B)
#define REG_LED1_OFF_L ((uint8_t)0x0C)
#define REG_LED1_OFF_H ((uint8_t)0x0D)
#define REG_LED2_ON_L ((uint8_t)0x0E)
#define REG_LED2_ON_H ((uint8_t)0x0F)

#define REG_LED2_OFF_L ((uint8_t)0x10)
#define REG_LED2_OFF_H ((uint8_t)0x11)
#define REG_LED3_ON_L ((uint8_t)0x12)
#define REG_LED3_ON_H ((uint8_t)0x13)
#define REG_LED3_OFF_L ((uint8_t)0x14)
#define REG_LED3_OFF_H ((uint8_t)0x15)
#define REG_LED4_ON_L ((uint8_t)0x16)
#define REG_LED4_ON_H ((uint8_t)0x17)
#define REG_LED4_OFF_L ((uint8_t)0x18)
#define REG_LED4_OFF_H ((uint8_t)0x19)
#define REG_LED5_ON_L ((uint8_t)0x1A)
#define REG_LED5_ON_H ((uint8_t)0x1B)
#define REG_LED5_OFF_L ((uint8_t)0x1C)
#define REG_LED5_OFF_H ((uint8_t)0x1D)
#define REG_LED6_ON_L ((uint8_t)0x1E)
#define REG_LED6_ON_H ((uint8_t)0x1F)

#define REG_LED6_OFF_L ((uint8_t)0x20)
#define REG_LED6_OFF_H ((uint8_t)0x21)
#define REG_LED7_ON_L ((uint8_t)0x22)
#define REG_LED7_ON_H ((uint8_t)0x23)
#define REG_LED7_OFF_L ((uint8_t)0x24)
#define REG_LED7_OFF_H ((uint8_t)0x25)
#define REG_LED8_ON_L ((uint8_t)0x26)
#define REG_LED8_ON_H ((uint8_t)0x27)
#define REG_LED8_OFF_L ((uint8_t)0x28)
#define REG_LED8_OFF_H ((uint8_t)0x29)
#define REG_LED9_ON_L ((uint8_t)0x2A)
#define REG_LED9_ON_H ((uint8_t)0x2B)
#define REG_LED9_OFF_L ((uint8_t)0x2C)
#define REG_LED9_OFF_H ((uint8_t)0x2D)
#define REG_LED10_ON_L ((uint8_t)0x2E)
#define REG_LED10_ON_H ((uint8_t)0x2F)

#define REG_LED10_OFF_L ((uint8_t)0x30)
#define REG_LED10_OFF_H ((uint8_t)0x31)
#define REG_LED11_ON_L ((uint8_t)0x32)
#define REG_LED11_ON_H ((uint8_t)0x33)
#define REG_LED11_OFF_L ((uint8_t)0x34)
#define REG_LED11_OFF_H ((uint8_t)0x35)
#define REG_LED12_ON_L ((uint8_t)0x36)
#define REG_LED12_ON_H ((uint8_t)0x37)
#define REG_LED12_OFF_L ((uint8_t)0x38)
#define REG_LED12_OFF_H ((uint8_t)0x39)
#define REG_LED13_ON_L ((uint8_t)0x3A)
#define REG_LED13_ON_H ((uint8_t)0x3B)
#define REG_LED13_OFF_L ((uint8_t)0x3C)
#define REG_LED13_OFF_H ((uint8_t)0x3D)
#define REG_LED14_ON_L ((uint8_t)0x3E)
#define REG_LED14_ON_H ((uint8_t)0x3F)

#define REG_LED14_OFF_L ((uint8_t)0x40)
#define REG_LED14_OFF_H ((uint8_t)0x41)
#define REG_LED15_ON_L ((uint8_t)0x42)
#define REG_LED15_ON_H ((uint8_t)0x43)
#define REG_LED15_OFF_L ((uint8_t)0x44)
#define REG_LED15_OFF_H ((uint8_t)0x45)

#define REG_ALL_LED_ON_L ((uint8_t)0xFA)
#define REG_ALL_LED_ON_H ((uint8_t)0xFB)
#define REG_ALL_LED_OFF_L ((uint8_t)0xFC)
#define REG_ALL_LED_OFF_H ((uint8_t)0xFD)
#define REG_PRE_SCALE ((uint8_t)0xFE)
#define REG_TEST_MODE ((uint8_t)0xFF)

#define RC_MODE1_RESTART ((uint8_t)0b10000000)
#define RC_MODE1_EXTCLK ((uint8_t)0b01000000)
#define RC_MODE1_AI ((uint8_t)0b00100000)
#define RC_MODE1_SLEEP ((uint8_t)0b00010000)
#define RC_MODE1_SUB1 ((uint8_t)0b00001000)
#define RC_MODE1_SUB2 ((uint8_t)0b00000100)
#define RC_MODE1_SUB3 ((uint8_t)0b00000010)
#define RC_MODE1_ALLCALL ((uint8_t)0b00000001)

#define WRITE_REG8(reg, val)          \
    {                                 \
        Wire.beginTransmission(addr); \
        Wire.write(reg);              \
        Wire.write(val);              \
        Wire.endTransmission();       \
    }

#define WRITE_REG16(reg, val)              \
    {                                      \
        Wire.beginTransmission(addr);      \
        Wire.write(reg);                   \
        Wire.write((uint8_t)(val));        \
        Wire.write((uint8_t)((val) >> 8)); \
        Wire.endTransmission();            \
    }

#define REG_ON_TIME(channel) (REG_LED0_ON_L + (channel * 4))
#define REG_OFF_TIME(channel) (REG_LED0_OFF_L + (channel * 4))

void AL_PCA9685::setup()
{
    pinMode(oePin, OUTPUT);
    WRITE_REG8(REG_MODE1, (RC_MODE1_AI | RC_MODE1_SLEEP));
    WRITE_REG8(REG_PRE_SCALE, AL_PCA9685_PRE_SCALE_50HZ);
    WRITE_REG8(REG_MODE1, RC_MODE1_AI);
}

void AL_PCA9685::setSleep(bool sleep)
{
    uint8_t regVal = sleep ? (RC_MODE1_AI | RC_MODE1_SLEEP) : RC_MODE1_AI;
    WRITE_REG8(REG_MODE1, regVal);
}

void AL_PCA9685::setPreScale(uint8_t /* AL_PCA9685_PRE_SCALE_x */ value)
{
    WRITE_REG8(REG_PRE_SCALE, value);
}

void AL_PCA9685::setOffTime(uint8_t channel, uint16_t /* [0,4096) */ value)
{
    WRITE_REG16(REG_OFF_TIME(channel), value);
}

void AL_PCA9685::setOnTime(uint8_t channel, uint16_t /* [0,4096) */ value)
{
    WRITE_REG16(REG_ON_TIME(channel), value);
}
