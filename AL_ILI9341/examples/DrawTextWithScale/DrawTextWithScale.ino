#include "AL_ILI9341.h"
#include "AL_Font.h"

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

AL_RgbColor backColor{255, 255, 255};
AL_RgbColor text1Color{255, 0, 0};
AL_RgbColor text2Color{0, 255, 0};
AL_RgbColor text3Color{0, 0, 255};
AL_RgbColor text4Color{0, 255, 255};
AL_RgbColor text5Color{255, 255, 0};

void setup()
{
    tft.setup();
    tft.setOrientation(AL_SO_LANDSCAPE2);
    tft.fillRect(0, 0, tft.getWidth(), tft.getHeight(), backColor);

    tft.drawText(10, 10, text1Color, backColor, 1, "hello");
    tft.drawText(10, 26, text2Color, backColor, 2, "hello");
    tft.drawText(10, 58, text3Color, backColor, 3, "hello");
    tft.drawText(10, 106, text4Color, backColor, 4, "hello");
    tft.drawText(10, 170, text5Color, backColor, 5, "hello");
}

void loop()
{
}