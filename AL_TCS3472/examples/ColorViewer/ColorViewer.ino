#include <Wire.h>
#include "AL_TCS3472.h"
#include "AL_ILI9341.h"

#define CS_ADDR 0x29
#define CS_LED_PIN 13

#define TFT_PORT PORTF
#define TFT_PIN PINF
#define TFT_DDR DDRF
#define TFT_RST A12
#define TFT_CS A11
#define TFT_RS A10
#define TFT_WR A9
#define TFT_RD A8

AL_TCS3472 cs(CS_ADDR, CS_LED_PIN);

AL_ILI9341 tft(
    &TFT_PORT, &TFT_PIN, &TFT_DDR,
    TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);

AL_RgbColor backColor(255, 255, 255);

void setup()
{
    Serial.begin(9600);

    Wire.begin();
    cs.setup();
    cs.powerOn(true);

    tft.setup();
    tft.setOrientation(AL_SO_LANDSCAPE2);
    tft.fillRect(0, 0, tft.getWidth(), tft.getHeight(), backColor);
}

void loop()
{
    AL_RgbColor color = cs.read();
    float hue = color.getHue();

    tft.fillRect(120, 80, 80, 80, color);

    delay(200);
}