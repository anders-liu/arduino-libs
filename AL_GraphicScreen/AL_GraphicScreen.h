//
// AL_GraphicScreen.h
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#ifndef AL_GRAPHICSCREEN__H
#define AL_GRAPHICSCREEN__H

#include <Arduino.h>
#include <stdint.h>

#include "AL_Color.h"

// Screen Orientations
#define AL_SO_PORTRAIT1 ((uint8_t)0)
#define AL_SO_PORTRAIT2 ((uint8_t)1)
#define AL_SO_LANDSCAPE1 ((uint8_t)2)
#define AL_SO_LANDSCAPE2 ((uint8_t)3)

class AL_GraphScreen
{
  protected:
    AL_GraphScreen(
        uint16_t longSide, uint16_t shortSide,
        uint8_t /* AL_SO_x */ orientation)
        : longSide(longSide), shortSide(shortSide),
          orientation(orientation)
    {
        setSize();
    }

  public:
    uint16_t getLongSide() { return longSide; }
    uint16_t getShortSide() { return shortSide; }
    uint16_t getWidth() { return width; }
    uint16_t getHeight() { return height; }
    uint8_t /* AL_SO_x */ getOrientation() { return orientation; }
    void setOrientation(uint8_t /* AL_SO_x */ o)
    {
        orientation = o;
        setSize();
        changeOrientation();
    }

  public:
    virtual void setup() = 0;

    virtual void fillRect(
        uint16_t x, uint16_t y,
        uint16_t w, uint16_t h,
        AL_RgbColor color) = 0;

    virtual void drawText(
        uint16_t x, uint16_t y,
        AL_RgbColor foreColor, AL_RgbColor backColor,
        const char *str, uint8_t maxLen) = 0;

  protected:
    // Must overwrite in subclass to actually change orientation.
    virtual void changeOrientation() = 0;

  private:
    uint16_t longSide, shortSide, width, height;
    uint8_t /* AL_SO_x */ orientation;

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

    void setUpdateArea(
        uint16_t x, uint16_t y,
        uint16_t w, uint16_t h);
};

#endif // AL_GRAPHICSCREEN__H
