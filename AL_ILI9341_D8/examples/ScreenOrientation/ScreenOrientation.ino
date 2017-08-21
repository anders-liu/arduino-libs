#include "AL_ILI9341_D8.h"

// Wiring
#define TFT_PORT PORTF
#define TFT_PIN PINF
#define TFT_DDR DDRF
#define TFT_RST A8
#define TFT_CS A9
#define TFT_RS A10
#define TFT_WR A11
#define TFT_RD A12

AL_ILI9341_D8 tft(
    &TFT_PORT, &TFT_PIN, &TFT_DDR,
    TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);

uint32_t lastMillis = 0;
AL_RgbColor backColor = {255, 255, 255};
AL_RgbColor tlColor = {255, 0, 0};
AL_RgbColor trColor = {0, 255, 0};
AL_RgbColor blColor = {0, 0, 255};
AL_RgbColor brColor = {255, 255, 0};

void testOrientation(AL_ScreenOrientation orientation)
{
    tft.setOrientation(orientation);
    tft.clear(backColor);

    uint16_t w = tft.getWidth(), h = tft.getHeight();
    tft.drawText(10, 10, tlColor, backColor, "TL");
    tft.drawText(w - 26, 10, trColor, backColor, "TR");
    tft.drawText(10, h - 26, blColor, backColor, "BL");
    tft.drawText(w - 26, h - 26, brColor, backColor, "BR");
}

void setup()
{
    tft.setup();
}

void loop()
{
    testOrientation(AL_SO_LANDSCAPE1);
    delay(1000);

    testOrientation(AL_SO_LANDSCAPE2);
    delay(1000);

    testOrientation(AL_SO_PORTRAIT1);
    delay(1000);

    testOrientation(AL_SO_PORTRAIT2);
    delay(1000);
}