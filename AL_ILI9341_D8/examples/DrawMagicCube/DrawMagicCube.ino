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
    TFT_RST, TFT_CS, TFT_RS, TFT_WR, TFT_RD,
    AL_SO_LANDSCAPE2);

uint32_t lastMillis = 0;

#define PADDING_SZ 1
#define SPACING_SZ 2
#define TILE_SZ 18
#define FACE_SZ /* TILE_SZ * 3 + 4 */ 58
#define FACE_INNER_SZ /* TILE_SZ * 3 + 2 */ 56

#define CUBE_W 180
#define CUBE_H 240

#define CUBE_FS1 1
#define CUBE_FS2 61
#define CUBE_FS3 121
#define CUBE_FS4 181

AL_RgbColor cubeAreaColor = {255, 255, 255};
AL_RgbColor lineColor = {0xF8, 0xF8, 0xF8};
AL_RgbColor fTileColor = {255, 0, 0};     // red
AL_RgbColor bTileColor = {237, 191, 0};   // orange
AL_RgbColor lTileColor = {0, 0, 255};     // blue
AL_RgbColor rTileColor = {0, 255, 0};     // green
AL_RgbColor uTileColor = {255, 255, 0};   //yellow
AL_RgbColor dTileColor = {255, 255, 255}; //white

void drawFace(uint16_t x0, uint16_t y0, AL_RgbColor color)
{
    uint16_t x, y;

    // Top line.
    tft.fillRect(x0, y0, FACE_SZ, 1, lineColor);

    // Bottom line.
    tft.fillRect(x0, y0 + FACE_SZ - 1, FACE_SZ, 1, lineColor);

    // Left line.
    tft.fillRect(x0, y0 + 1, 1, FACE_SZ - 2, lineColor);

    // Right line.
    tft.fillRect(x0 + FACE_SZ - 1, y0 + 1, 1, FACE_SZ - 2, lineColor);

    // Inner H lines.
    x = x0 + 1;
    y = y0 + 1 + TILE_SZ;
    tft.fillRect(x, y, FACE_INNER_SZ, 1, lineColor);
    y += 1 + TILE_SZ;
    tft.fillRect(x, y, FACE_INNER_SZ, 1, lineColor);
    y += 1 + TILE_SZ;
    tft.fillRect(x, y, FACE_INNER_SZ, 1, lineColor);

    // Inner V liens.
    y = y0 + 1;
    x = x0 + 1 + TILE_SZ;
    tft.fillRect(x, y, 1, FACE_INNER_SZ, lineColor);
    x += 1 + TILE_SZ;
    tft.fillRect(x, y, 1, FACE_INNER_SZ, lineColor);
    x += 1 + TILE_SZ;
    tft.fillRect(x, y, 1, FACE_INNER_SZ, lineColor);

    // Tiles.
    for (uint8_t xi = 0; xi < 3; xi++)
    {
        for (uint8_t yi = 0; yi < 3; yi++)
        {
            x = x0 + 1 + xi * (TILE_SZ + 1);
            y = y0 + 1 + yi * (TILE_SZ + 1);
            tft.fillRect(x, y, TILE_SZ, TILE_SZ, color);
        }
    }
}

void drawCube()
{
    // Fill screen;
    tft.fillRect(0, 0, CUBE_W, CUBE_H, cubeAreaColor);

    //
    //       +---+
    //       | B |
    //       +---+
    //       +---+
    //       | U |
    //       +---+
    // +---+ +---+ +---+
    // | L | | F | | R |
    // +---+ +---+ +---+
    //       +---+
    //       | D |
    //       +---+
    //
    //

    // B face
    drawFace(CUBE_FS2, CUBE_FS1, bTileColor);

    // U face
    drawFace(CUBE_FS2, CUBE_FS2, uTileColor);

    // F face
    drawFace(CUBE_FS2, CUBE_FS3, fTileColor);

    // D face
    drawFace(CUBE_FS2, CUBE_FS4, dTileColor);

    // L face
    drawFace(CUBE_FS1, CUBE_FS3, lTileColor);

    // R face
    drawFace(CUBE_FS3, CUBE_FS3, rTileColor);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    Serial.println("Serial port ready.");

    tft.setup();
    Serial.println("TFT has been setup.");

    drawCube();
}

void loop()
{
    uint32_t ms = millis();
    if (ms - lastMillis > 500)
    {
        uint32_t id = tft.readID();
        uint32_t ds = tft.readStatus();
        uint8_t pm = tft.readPowerMode();
        Serial.println(id, HEX);
        Serial.println(ds, HEX);
        Serial.println(pm, HEX);

        lastMillis = ms;
    }
}