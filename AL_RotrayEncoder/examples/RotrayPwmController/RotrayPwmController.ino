#include <stdio.h>
#include <Wire.h>

#include "AL_ILI9341.h"
#include "AL_RotrayEncoder.h"
#include "AL_PCA9685.h"

//
// Wiring
//

// TFT
#define TFT_PORT PORTF
#define TFT_PIN PINF
#define TFT_DDR DDRF
#define TFT_RST A12
#define TFT_CS A11
#define TFT_RS A10
#define TFT_WR A9
#define TFT_RD A8

// Rotray encoder
#define ENC_A_PIN 49
#define ENC_B_PIN 48
#define ENC_SW_PIN 47

// PWM
#define PWM_ADDR 0x40
#define PWM_OE_PIN 12

//
// Devices
//

AL_ILI9341 tft(
    &TFT_PORT, &TFT_PIN, &TFT_DDR,
    TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);

AL_RotrayEncoder enc(ENC_A_PIN, ENC_B_PIN);

AL_PCA9685 pwm(PWM_ADDR, PWM_OE_PIN);

//
// Variables
//

AL_RgbColor backColor(0, 0, 0);
AL_RgbColor textColor(255, 255, 0);

uint16_t offTimeValue = 307; // default: 1.5ms

#define SCALE 3
#define TEXT_X ((320 - SCALE * 4 * AL_FONT_CHAR_W) / 2)
#define TEXT_Y ((240 - SCALE * AL_FONT_CHAR_H) / 2)

#define PWM_CHANNEL 8

char valueStr[20];

void drawValue()
{
    snprintf(valueStr, 20, "%4d", offTimeValue);
    tft.drawText(TEXT_X, TEXT_Y, textColor, backColor, SCALE, valueStr);
}

void onClockwiseTick()
{
    offTimeValue+=10;
    pwm.setOffTime(PWM_CHANNEL, offTimeValue);
    drawValue();
}

void onCounterclockwiseTick()
{
    offTimeValue-=10;
    pwm.setOffTime(PWM_CHANNEL, offTimeValue);
    drawValue();
}

void setup()
{
    Wire.begin();

    tft.setup();
    tft.setOrientation(AL_SO_LANDSCAPE2);
    tft.displayOff();
    tft.fillRect(0, 0, tft.getWidth(), tft.getHeight(), backColor);
    tft.displayOn();

    enc.setup();
    enc.handleClockwiseTick(onClockwiseTick);
    enc.handleCounterclockwiseTick(onCounterclockwiseTick);

    pwm.setup();
    pwm.setOutputEnable(true);
    pwm.setOffTime(PWM_CHANNEL, offTimeValue);

    drawValue();
}

void loop()
{
    enc.loop();
}