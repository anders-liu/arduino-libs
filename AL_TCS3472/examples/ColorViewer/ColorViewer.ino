#include <Arduino.h>
#include <Wire.h>
#include "AL_TCS3472.h"
#include "AL_ILI9341.h"

#define CS_ADDR 0x29
#define CS_LED_PIN 22

#define TFT_PORT PORTF
#define TFT_PIN PINF
#define TFT_DDR DDRF
#define TFT_RST A9
#define TFT_CS A8
#define TFT_RS A10
#define TFT_WR A11
#define TFT_RD A12

AL_TCS3472 cs(CS_ADDR, CS_LED_PIN);

AL_ILI9341 tft(
    &TFT_PORT, &TFT_PIN, &TFT_DDR,
    TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);

AL_RgbColor backColor(0, 0, 0);
AL_RgbColor textColor(255, 255, 255);

#define STR_BUF_SZ 10

void setup()
{
    Serial.begin(9600);

    Wire.begin();
    cs.setup();
    cs.powerOn(true);

    digitalWrite(CS_LED_PIN, LOW);
    delay(300);
    digitalWrite(CS_LED_PIN, HIGH);
    delay(300);
    digitalWrite(CS_LED_PIN, LOW);
    delay(300);
    digitalWrite(CS_LED_PIN, HIGH);
    delay(300);
    digitalWrite(CS_LED_PIN, LOW);
    delay(300);
    digitalWrite(CS_LED_PIN, HIGH);

    tft.setup();
    tft.setOrientation(AL_SO_LANDSCAPE2);
    tft.fillRect(0, 0, tft.getWidth(), tft.getHeight(), backColor);

    // draw labels
    tft.drawText(8, 16, textColor, backColor, "Raw: C=     , R=     , G=     , B=");
    tft.drawText(8, 32, textColor, backColor, "RGB1: R=   , G=   , B=");
    tft.drawText(8, 48, textColor, backColor, "HSV: H=     , S=    , V=");
    tft.drawText(8, 64, textColor, backColor, "RGB2: R=   , G=   , B=");

    // draw color box borders
    tft.fillRect(40, 120, 1, 80, textColor);  // left
    tft.fillRect(40, 120, 80, 1, textColor);  // top
    tft.fillRect(119, 120, 1, 80, textColor); // right
    tft.fillRect(40, 199, 80, 1, textColor);  // bottom

    tft.fillRect(200, 120, 1, 80, textColor); // left
    tft.fillRect(200, 120, 80, 1, textColor); // top
    tft.fillRect(279, 120, 1, 80, textColor); // right
    tft.fillRect(200, 199, 80, 1, textColor); // bottom
}

void loop()
{
    AL_RawColor raw = cs.readRaw();
    AL_RgbColor rgb = cs.readRgb();
    AL_HsvColor hsv = rgb2hsv(rgb);
    AL_RgbColor rgb2 = hsv2rgb(hsv);

    // draw color values
    char buffer[STR_BUF_SZ] = {0};

    snprintf(buffer, STR_BUF_SZ - 1, "%5u", raw.c);
    tft.drawText(64, 16, textColor, backColor, buffer);
    snprintf(buffer, STR_BUF_SZ - 1, "%5u", raw.r);
    tft.drawText(136, 16, textColor, backColor, buffer);
    snprintf(buffer, STR_BUF_SZ - 1, "%5u", raw.g);
    tft.drawText(208, 16, textColor, backColor, buffer);
    snprintf(buffer, STR_BUF_SZ - 1, "%5u", raw.b);
    tft.drawText(280, 16, textColor, backColor, buffer);

    snprintf(buffer, STR_BUF_SZ - 1, "%3u", rgb.r);
    tft.drawText(72, 32, textColor, backColor, buffer);
    snprintf(buffer, STR_BUF_SZ - 1, "%3u", rgb.g);
    tft.drawText(128, 32, textColor, backColor, buffer);
    snprintf(buffer, STR_BUF_SZ - 1, "%3u", rgb.b);
    tft.drawText(184, 32, textColor, backColor, buffer);

    uint16_t temp;
    temp = hsv.h * 10;

    snprintf(buffer, STR_BUF_SZ - 1, "%4u", temp);
    buffer[5] = buffer[4];
    buffer[4] = buffer[3];
    buffer[3] = '.';
    tft.drawText(64, 48, textColor, backColor, buffer);

    temp = hsv.s * 100;
    snprintf(buffer, STR_BUF_SZ - 1, "%3u", temp);
    buffer[4] = buffer[3];
    buffer[3] = buffer[2];
    buffer[2] = buffer[1];
    buffer[1] = '.';
    tft.drawText(136, 48, textColor, backColor, buffer);

    temp = hsv.v * 100;
    snprintf(buffer, STR_BUF_SZ - 1, "%3u", temp);
    buffer[4] = buffer[3];
    buffer[3] = buffer[2];
    buffer[2] = buffer[1];
    buffer[1] = '.';
    tft.drawText(200, 48, textColor, backColor, buffer);

    snprintf(buffer, STR_BUF_SZ - 1, "%3u", rgb2.r);
    tft.drawText(72, 64, textColor, backColor, buffer);
    snprintf(buffer, STR_BUF_SZ - 1, "%3u", rgb2.g);
    tft.drawText(128, 64, textColor, backColor, buffer);
    snprintf(buffer, STR_BUF_SZ - 1, "%3u", rgb2.b);
    tft.drawText(184, 64, textColor, backColor, buffer);

    // draw color boxes
    tft.fillRect(41, 121, 78, 78, rgb);
    tft.fillRect(201, 121, 78, 78, rgb2);

    delay(200);
}