#include "AL_ILI9341.h"
#include "AL_TouchScreen.h"

// Wiring
#define TFT_PORT PORTF
#define TFT_PIN PINF
#define TFT_DDR DDRF
#define TFT_RST A8
#define TFT_CS A9
#define TFT_RS A10
#define TFT_WR A11
#define TFT_RD A12

// For ILI9341
#define TC_XP /* TFT_D0 */ A0
#define TC_XM /* TFT_RS */ A10
#define TC_YP /* TFT_CS */ A9
#define TC_YM /* TFT_D1 */ A1

AL_ILI9341 tft(
    &TFT_PORT, &TFT_PIN, &TFT_DDR,
    TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);

AL_TouchScreen ts(
    TC_XP, TC_XM, TC_YP, TC_YM,
    tft.getLongSide(), tft.getShortSide());

uint16_t lastX, lastY;
AL_RgbColor backColor{255, 255, 255};
AL_RgbColor headColor{255, 0, 0};
AL_RgbColor lineColor{0, 0, 255};

#define DOT_SZ 8

void drawDot(uint16_t x, uint16_t y)
{
    tft.fillRect(x - DOT_SZ / 2, y - DOT_SZ / 2, DOT_SZ, DOT_SZ, lineColor);
}

void drawHead(uint16_t x, uint16_t y)
{
    tft.fillRect(x - DOT_SZ / 2, y - DOT_SZ / 2, DOT_SZ, DOT_SZ, headColor);
}

void onTouchStart(uint16_t x, uint16_t y)
{
    drawHead(x, y);
    lastX = x, lastY = y;
}

void onTouchMove(uint16_t x, uint16_t y)
{
    drawDot(lastX, lastY);
    drawHead(x, y);
    lastX = x, lastY = y;
}

void onTouchEnd()
{
    drawDot(lastX, lastY);
}

void setup()
{
    uint8_t orientation = AL_SO_PORTRAIT1;

    ts.setOrientation(orientation);
    ts.handleTouchStart(onTouchStart);
    ts.handleTouchMove(onTouchMove);
    ts.handleTouchEnd(onTouchEnd);

    tft.setup();
    tft.setOrientation(orientation);
    tft.fillRect(0, 0, tft.getWidth(), tft.getHeight(), backColor);
    tft.drawText(10, 10, lineColor, backColor, "Hi");
}

void loop()
{
    ts.loop();
}