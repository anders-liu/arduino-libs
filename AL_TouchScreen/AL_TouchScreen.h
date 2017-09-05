//
// AL_TouchScreen.h
//
// Implement resistive touch screen on graphic screen module.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#ifndef AL_TOUCHSCREEN__H
#define AL_TOUCHSCREEN__H

#include <Arduino.h>
#include <stdint.h>

#include "AL_GraphicScreen.h"

typedef void (*AL_TouchScreenPosEventHandler)(uint16_t x, uint16_t y);
typedef void (*AL_TouchScreenEventHandler)();

class AL_TouchScreen
{
public:
  AL_TouchScreen(
      uint8_t xpPin, uint8_t xmPin, uint8_t ypPin, uint8_t ymPin,
      uint16_t longSide, uint16_t shortSide)
      : xpPin(xpPin), xmPin(xmPin), ypPin(ypPin), ymPin(ymPin),
        longSide(longSide), shortSide(shortSide),
        width(shortSide), height(longSide),
        orientation(AL_SO_PORTRAIT1),
        state(0), curX(0), curY(0), lastMillis(0),
        touchStartHandler(nullptr), touchEndHandler(nullptr)
  {
  }

public:
  uint16_t getWidth() { return width; }
  uint16_t getHeight() { return height; }
  uint8_t /* AL_SO_x */ getOrientation() { return orientation; }
  void setOrientation(uint8_t /* AL_SO_x */ o)
  {
    switch (o)
    {
    case AL_SO_PORTRAIT1:
    case AL_SO_PORTRAIT2:
      width = shortSide;
      height = longSide;
      break;
    case AL_SO_LANDSCAPE1:
    case AL_SO_LANDSCAPE2:
      width = longSide;
      height = shortSide;
      break;
    }
    orientation = o;
  }

  void loop();

public:
  void handleTouchStart(AL_TouchScreenPosEventHandler handler) { touchStartHandler = handler; }
  void handleTouchMove(AL_TouchScreenPosEventHandler handler) { touchMoveHandler = handler; }
  void handleTouchEnd(AL_TouchScreenEventHandler handler) { touchEndHandler = handler; }

private:
  void read();

private:
  uint8_t xpPin, xmPin, ypPin, ymPin;
  uint16_t longSide, shortSide, width, height;
  uint8_t /* AL_SO_x */ orientation;
  uint8_t state;
  uint16_t curX, curY;
  uint32_t lastMillis;

  AL_TouchScreenPosEventHandler touchStartHandler;
  AL_TouchScreenPosEventHandler touchMoveHandler;
  AL_TouchScreenEventHandler touchEndHandler;
};

#endif // AL_TOUCHSCREEN__H
