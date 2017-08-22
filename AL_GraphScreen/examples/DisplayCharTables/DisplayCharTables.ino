#include "AL_ILI9481_D8.h"

//
// Wiring.
//
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
    TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD,
    AL_SO_LANDSCAPE2);

void setup()
{
    tft.setup();
    tft.clear({255, 255, 255});
}

void loop()
{
}