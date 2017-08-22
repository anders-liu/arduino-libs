//
// Copyright Anders Liu.
//
// Graphical screen interfaces.
//
// For more information, visit https://github.com/anders-liu/arduino-libs/tree/master/AL_GraphScreen
//

#ifndef AL_GRAPHSCREEN__H
#define AL_GRAPHSCREEN__H

#include <Arduino.h>
#include <stdint.h>

// Provided by application.
#include "AL_GraphScreen_Config.h"

#include "Font.h"

#ifndef AL_RGBCOLOR_STRUCT
#define AL_RGBCOLOR_STRUCT

struct AL_RgbColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    uint16_t toColor565()
    {
        return ((r & B11111000) << 8) | ((g & B11111100) << 3) | (b >> 3);
    }
};

#endif // AL_RGBCOLOR_STRUCT

enum AL_ScreenOrientation
{
    AL_SO_LANDSCAPE1 = 0,
    AL_SO_LANDSCAPE2 = 1,
    AL_SO_PORTRAIT1 = 2,
    AL_SO_PORTRAIT2 = 3,

    AL_SO_HORIZONTAL1 = AL_SO_LANDSCAPE1,
    AL_SO_HORIZONTAL2 = AL_SO_LANDSCAPE2,
    AL_SO_VERTICAL1 = AL_SO_PORTRAIT1,
    AL_SO_VERTICAL2 = AL_SO_PORTRAIT2,
};

#define AL_GS_FONT_ID_5X7 0
#define AL_GS_FONT_ID_8X12 1
#define AL_GS_FONT_ID_8X16 2

class AL_GraphScreen
{
  public:
    uint16_t getLongSide() { return longSide; }
    uint16_t getShortSide() { return shortSide; }
    uint16_t getWidth() { return width; }
    uint16_t getHeight() { return height; }
    AL_ScreenOrientation getOrientation() { return orientation; }
    void setOrientation(AL_ScreenOrientation o)
    {
        orientation = o;
        setSize();
        changeOrientation();
    }

    virtual void displayOff() = 0;
    virtual void displayOn() = 0;
    virtual void sleepIn() = 0;
    virtual void sleepOut() = 0;
    virtual void clear(AL_RgbColor color) = 0;

    virtual void fillRect(
        uint16_t x, uint16_t y,
        uint16_t w, uint16_t h,
        AL_RgbColor color) = 0;

#if defined(TEXT_ENABLED)
#if !defined(ONLY_ONE_FONT)
    void setFont(uint8_t /* AL_GS_FONT_ID_x */ fontID)
    {
        curFontID = fontID;
        switch (fontID)
        {
#ifdef AL_GS_ENABLE_FONT_5X7
        case AL_GS_FONT_ID_5X7:
            fontBytesPerChar = FONT_BYTES_PER_CHAR_5X7;
            fontData = font5x7Data;
            charWidth = CHAR_W_5X7;
            charHeight = CHAR_H_5X7;
            break;
#endif
#ifdef AL_GS_ENABLE_FONT_8X12
        case AL_GS_FONT_ID_8X12:
            fontBytesPerChar = FONT_BYTES_PER_CHAR_8X12;
            fontData = font8x12Data;
            charWidth = CHAR_W_8X12;
            charHeight = CHAR_H_8X12;
            break;
#endif
#ifdef AL_GS_ENABLE_FONT_8X16
        case AL_GS_FONT_ID_8X16:
            fontBytesPerChar = FONT_BYTES_PER_CHAR_8X16;
            fontData = font8x16Data;
            charWidth = CHAR_W_8X16;
            charHeight = CHAR_H_8X16;
            break;
#endif
        }
    }
    uint8_t /* AL_GS_FONT_ID_x */ getFont() { return curFontID; }
#endif // !defined(ONLY_ONE_FONT)

    virtual void drawText(
        uint16_t x, uint16_t y,
        AL_RgbColor foreColor, AL_RgbColor backColor,
        const char *str, uint8_t maxLen) = 0;
#endif // TEXT_ENABLED

  protected:
    AL_GraphScreen(
        uint16_t longSide, uint16_t shortSide,
        AL_ScreenOrientation orientation)
        : longSide(longSide), shortSide(shortSide),
          orientation(orientation)
    {
        setSize();
    }

    // Must overwrite in subclass to actually change orientation.
    virtual void changeOrientation() = 0;

#if defined(TEXT_ENABLED) && !defined(ONLY_ONE_FONT)
    uint8_t curFontID;

    uint8_t fontBytesPerChar;
    PGM_P fontData;
    uint8_t charWidth;
    uint8_t charHeight;
#else
#define fontBytesPerChar ONLY_ONE_FONT_BYTES_PER_CHAR
#define fontData ONLY_ONE_FONT_DATA
#define charWidth ONLY_ONE_FONT_CHAR_W
#define charHeight ONLY_ONE_FONT_CHAR_H
#endif

  private:
    uint16_t longSide, shortSide, width, height;
    AL_ScreenOrientation orientation;

    void setSize()
    {
        switch (orientation)
        {
        case AL_SO_LANDSCAPE1:
        case AL_SO_LANDSCAPE2:
            width = longSide;
            height = shortSide;
            break;
        case AL_SO_PORTRAIT1:
        case AL_SO_PORTRAIT2:
            width = shortSide;
            height = longSide;
            break;
        }
    }
};

#endif // AL_GRAPHSCREEN__H
