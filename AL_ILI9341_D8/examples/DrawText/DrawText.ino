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

AL_ILI9341_D8 tft(&TFT_PORT, &TFT_PIN, &TFT_DDR, TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);

uint32_t lastMillis = 0;
RgbColor backColor = {255, 255, 255};
RgbColor lineColor = {0, 0, 0};
RgbColor headerColor = {0, 0, 255};
RgbColor valueColor = {82, 200, 3};

#define SCREEN_W 320
#define SCREEN_H 240

#define TOTAL_ROWS 9
#define TOTAL_COLS 34

#define PADDING_T 3
#define PADDING_L 7
#define GRID_PADDING_T 5
#define GRID_PADDING_L 0
#define GRID_W 9
#define GRID_H 26

const static char hex[] = "0123456789ABCDEF";

void drawChar(uint8_t col, uint8_t row, RgbColor color, char c)
{
    char str[] = {c, 0};
    tft.drawText(
        PADDING_L + col * GRID_W + 1 + GRID_PADDING_L,
        PADDING_T + row * GRID_H + 1 + GRID_PADDING_T,
        color, backColor, str);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    Serial.println("Serial port ready.");

    tft.setup();
    Serial.println("TFT has been setup.");

    // Clear screen.
    tft.fillRect(0, 0, SCREEN_W, SCREEN_H, backColor);

    // Draw H lines.
    for (uint8_t r = 0; r < TOTAL_ROWS - 1; r++)
    {
        tft.fillRect(0, PADDING_T + (r + 1) * GRID_H, SCREEN_W, 1, lineColor);
    }

    // Draw V liens.
    for (uint8_t c = 0; c < TOTAL_COLS - 1; c++)
    {
        tft.fillRect(PADDING_L + (c + 1) * GRID_W, 0, 1, SCREEN_H, lineColor);
    }

    // Draw column headers.
    for (uint8_t c = 0; c < TOTAL_COLS - 2; c++)
    {
        drawChar(c + 2, 0, headerColor, hex[c & 0x0F]);
    }

    // Draw row headers.
    for (uint8_t r = 0; r < TOTAL_ROWS - 1; r++)
    {
        char c1 = hex[(r * 2) & 0x0F];
        char c2 = hex[(r * 2 + 1) & 0xF];
        drawChar(0, r + 1, headerColor, c1);
        drawChar(1, r + 1, headerColor, c2);
    }

    // Draw chars.
    for (uint16_t v = 0; v < 256; v++)
    {
        drawChar(2 + v % (TOTAL_COLS - 2), 1 + v / (TOTAL_COLS - 2), valueColor, (char)v);
    }
}

void loop()
{
}