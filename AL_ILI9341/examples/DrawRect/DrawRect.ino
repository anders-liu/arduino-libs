#include "AL_ILI9341.h"

// Wiring
#define TFT_PORT PORTF
#define TFT_PIN PINF
#define TFT_DDR DDRF
#define TFT_RST A9
#define TFT_CS A8
#define TFT_RS A10
#define TFT_WR A11
#define TFT_RD A12

AL_ILI9341 tft(
    &TFT_PORT, &TFT_PIN, &TFT_DDR,
    TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);

void setup()
{
    tft.setup();
    tft.setOrientation(AL_SO_LANDSCAPE1);

    uint16_t x = 0, y = 0;
    uint16_t w = tft.getWidth() / 4;
    uint16_t h = tft.getHeight() / 2;

    tft.fillRect(x, y, w, h, {255, 255, 255});
    x += w;
    tft.fillRect(x, y, w, h, {255, 0, 0});
    x += w;
    tft.fillRect(x, y, w, h, {0, 255, 0});
    x += w;
    tft.fillRect(x, y, w, h, {0, 0, 255});
    x = 0;
    y += h;
    tft.fillRect(x, y, w, h, {0, 0, 0});
    x += w;
    tft.fillRect(x, y, w, h, {0, 255, 255});
    x += w;
    tft.fillRect(x, y, w, h, {255, 0, 255});
    x += w;
    tft.fillRect(x, y, w, h, {255, 255, 0});
}

void loop()
{
}