/*
 * Copyright Anders Liu.
 * 
 * Graphical screen interfaces.
 *
 * For more information, visit https://github.com/anders-liu/arduino-libs/tree/master/AL_GraphScreen
 */

#pragma once

#include <Arduino.h>
#include <stdint.h>

struct AL_RgbColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

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
        changeOrientation(o);
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
    virtual void changeOrientation(AL_ScreenOrientation o) = 0;

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

class AL_GraphScreenWithText : public AL_GraphScreen
{
  public:
    virtual void drawText(
        uint16_t x, uint16_t y,
        AL_RgbColor foreColor, AL_RgbColor backColor,
        const char *str, uint8_t maxLen) = 0;
};