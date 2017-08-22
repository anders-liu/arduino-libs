//
// Copyright Anders Liu.
//
// 3.5" TFT based on ILI9481 (320x480) with a single port for 8-bit data interface.
//
// For most 3.5 TFT on market, the data lines are separated into 2 ports and hard to program, and potentially low performance.
// So with this library, you need re-arrange pins with customized adapter pcb or flat cable.
//
// For more information, visit https://github.com/anders-liu/arduino-libs/tree/master/AL_GraphScreen
//

#ifndef AL_ILI9481_D8__H
#define AL_ILI9481_D8__H

#include <Arduino.h>
#include <stdint.h>

#include "AL_GraphScreen.h"

class AL_ILI9481_D8 : public AL_GraphScreen
{
  public:
    AL_ILI9481_D8(
        volatile uint8_t *pPort, volatile uint8_t *pPin, volatile uint8_t *pDdr,
        byte rstPin, byte csPin, byte rsPin, byte wrPin, byte rdPin,
        AL_ScreenOrientation orientation = AL_SO_PORTRAIT1)
        : AL_GraphScreen(480, 320, orientation),
          pPort(pPort), pPin(pPin), pDdr(pDdr),
          rstPin(rstPin), csPin(csPin), rsPin(rsPin), wrPin(wrPin), rdPin(rdPin)
    {
    }

    void setup();

    /**
      * Read display status (command 09h).
      * @returns DS_* constants.
      */
    uint32_t readStatus();

    /**
      * Read display identity (command D3h).
      * @returns 0x00009481.
      */
    uint32_t readID();

    /**
      * Read display power mode (command 0Ah).
      * @returns PM_* constants.
      */
    uint8_t readPowerMode();

    void displayOff();
    void displayOn();
    void sleepIn();
    void sleepOut();
    void clear(AL_RgbColor color);

    void changeOrientation();

    void fillRect(
        uint16_t x, uint16_t y,
        uint16_t w, uint16_t h,
        AL_RgbColor color);

#ifdef TEXT_ENABLED
    void drawText(
        uint16_t x, uint16_t y,
        AL_RgbColor foreColor, AL_RgbColor backColor,
        const char *str, uint8_t maxLen = 100);
#endif

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

#endif // AL_ILI9481_D8__H
