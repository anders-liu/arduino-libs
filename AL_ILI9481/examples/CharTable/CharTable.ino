#include "AL_ILI9481.h"
#include "AL_Font.h"

// Wiring
#define TFT_PORT PORTF
#define TFT_PIN PINF
#define TFT_DDR DDRF
#define TFT_RST A12
#define TFT_CS A11
#define TFT_RS A10
#define TFT_WR A9
#define TFT_RD A8

AL_ILI9481 tft(
    &TFT_PORT, &TFT_PIN, &TFT_DDR,
    TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD);

// Dependent on screen orientation, so it must be one of landscape.
#define SCREEN_W AL_SCREEN_LONG_SIDE
#define SCREEN_H AL_SCREEN_SHORT_SIDE

#define TABLE_ROWS 8
#define TABLE_COLS 32

#define COL_HEADER_CELLS 1
#define ROW_HEADER_CELLS 2

#define TOTAL_ROWS (TABLE_ROWS + COL_HEADER_CELLS)
#define TOTAL_COLS (TABLE_COLS + ROW_HEADER_CELLS)

#define LINE_SIZE 1

#define CELL_W (AL_FONT_CHAR_W + LINE_SIZE)
#define CELL_H (AL_FONT_CHAR_H + LINE_SIZE)

#define TABLE_W (LINE_SIZE + CELL_W * TOTAL_COLS)
#define TABLE_H (LINE_SIZE + CELL_H * TOTAL_ROWS)

#define PADDING_LEFT ((SCREEN_W - TABLE_W) / 2)
#define PADDING_TOP ((SCREEN_H - TABLE_H) / 2)

#define CELL_LEFT(col) (PADDING_LEFT + LINE_SIZE + (col)*CELL_W)
#define CELL_TOP(row) (PADDING_TOP + LINE_SIZE + (row)*CELL_H)

#define V_LINE_X(col) (CELL_LEFT(col) + AL_FONT_CHAR_W)
#define H_LINE_Y(row) (CELL_TOP(row) + AL_FONT_CHAR_H)

AL_RgbColor backColor{255, 255, 255};
AL_RgbColor lineColor{100, 100, 100};
AL_RgbColor headerColor{0, 255, 0};
AL_RgbColor charColor{255, 0, 0};

static const char hex[] = "0123456789ABCDEF";

void setup()
{
    tft.setup();
    tft.setOrientation(AL_SO_LANDSCAPE2);
    tft.fillRect(0, 0, SCREEN_W, SCREEN_H, backColor);

    uint16_t col, row, i;

    // V lines.
    tft.fillRect(PADDING_LEFT, PADDING_TOP, LINE_SIZE, TABLE_H, lineColor);
    for (col = 0; col < TOTAL_COLS; col++)
        tft.fillRect(V_LINE_X(col), PADDING_TOP, LINE_SIZE, TABLE_H, lineColor);

    // H lines.
    tft.fillRect(PADDING_LEFT, PADDING_TOP, TABLE_W, LINE_SIZE, lineColor);
    for (row = 0; row < TOTAL_ROWS; row++)
        tft.fillRect(PADDING_LEFT, H_LINE_Y(row), TABLE_W, LINE_SIZE, lineColor);

    // Column header.
    for (i = 0; i < TABLE_COLS; i++)
    {
        char c = hex[i & 0x0F];
        tft.drawText(
            CELL_LEFT(i + ROW_HEADER_CELLS), CELL_TOP(0),
            headerColor, backColor, &c, 1);
    }

    // Row header.
    for (row = 0; row < TABLE_ROWS; row++)
    {
        for (col = 0; col < ROW_HEADER_CELLS; col++)
        {
            char c = hex[(row * ROW_HEADER_CELLS + col) & 0x0F];
            tft.drawText(
                CELL_LEFT(col), CELL_TOP(row + COL_HEADER_CELLS),
                headerColor, backColor, &c, 1);
        }
    }

    // Chars.
    for (row = 0; row < TABLE_ROWS; row++)
    {
        for (col = 0; col < TABLE_COLS; col++)
        {
            char c = row * TABLE_COLS + col;
            tft.drawText(
                CELL_LEFT(col + ROW_HEADER_CELLS), CELL_TOP(row + COL_HEADER_CELLS),
                charColor, backColor, &c, 1);
        }
    }
}

void loop()
{
}