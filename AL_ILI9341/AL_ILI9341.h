//
// AL_ILI9341.h
//
// Implement graphic screen on AL_ILI9341 based TFT screen.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#ifndef AL_ILI9341__H
#define AL_ILI9341__H

#include "AL_GraphicScreen.h"
#include "AL_Font.h"

#define AL_SCREEN_LONG_SIDE 320
#define AL_SCREEN_SHORT_SIDE 240

class AL_ILI9341 : public AL_GraphScreen
{
public:
  AL_ILI9341(
      volatile uint8_t *pPort, volatile uint8_t *pPin, volatile uint8_t *pDdr,
      byte rstPin, byte csPin, byte rsPin, byte wrPin, byte rdPin,
      uint8_t /* AL_SO_x */ orientation = AL_SO_PORTRAIT1)
      : AL_GraphScreen(AL_SCREEN_LONG_SIDE, AL_SCREEN_SHORT_SIDE, orientation),
        pPort(pPort), pPin(pPin), pDdr(pDdr),
        rstPin(rstPin), csPin(csPin), rsPin(rsPin), wrPin(wrPin), rdPin(rdPin)
  {
  }

public:
  virtual void setup();

  virtual void fillRect(
      uint16_t x, uint16_t y,
      uint16_t w, uint16_t h,
      AL_RgbColor color);

  virtual void drawText(
      uint16_t x, uint16_t y,
      AL_RgbColor foreColor, AL_RgbColor backColor,
      const char *str, uint8_t maxLen = 100);

  virtual void drawText(
      uint16_t x, uint16_t y,
      AL_RgbColor foreColor, AL_RgbColor backColor,
      uint8_t scale, const char *str, uint8_t maxLen = 100);

protected:
  virtual void changeOrientation();

private:
  void setUpdateArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

  volatile uint8_t *pPort;
  volatile uint8_t *pPin;
  volatile uint8_t *pDdr;
  uint8_t rstPin;
  uint8_t csPin;
  uint8_t rsPin;
  uint8_t wrPin;
  uint8_t rdPin;
};

#endif //AL_ILI9341__H
