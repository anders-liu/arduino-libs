#include "AL_ILI9481_D8.h"

// Wiring
#define TFT_PORT PORTF
#define TFT_PIN PINF
#define TFT_DDR DDRF
#define TFT_RST A8
#define TFT_CS A9
#define TFT_RS A10
#define TFT_WR A11
#define TFT_RD A12

AL_ILI9481_D8 tft(
    &TFT_PORT, &TFT_PIN, &TFT_DDR,
    TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);

uint32_t lastMillis = 0;
AL_RgbColor backColor = {0, 255, 255};
AL_RgbColor rColor = {255, 0, 0};
AL_RgbColor gColor = {0, 255, 0};
AL_RgbColor bColor = {0, 0, 255};
AL_RgbColor yColor = {255, 255, 0};

void testOrientation(AL_ScreenOrientation orientation)
{
    tft.setOrientation(orientation);
    tft.clear(backColor);

    uint16_t w = tft.getWidth(), h = tft.getHeight();
    tft.drawText(10, 10, rColor, backColor, "TL (red)");
    tft.drawText(w - 90, 10, gColor, backColor, "TR (green)");
    tft.drawText(10, h - 26, bColor, backColor, "BL (blue)");
    tft.drawText(w - 98, h - 26, yColor, backColor, "BR (yellow)");
}

void setup()
{
    Serial.begin(9600);
    tft.setup();
}

void loop()
{
    // Should rotating clockwise.

    testOrientation(AL_SO_LANDSCAPE1);
    delay(1000);

    testOrientation(AL_SO_PORTRAIT1);
    delay(1000);

    testOrientation(AL_SO_LANDSCAPE2);
    delay(1000);

    testOrientation(AL_SO_PORTRAIT2);
    delay(1000);
}