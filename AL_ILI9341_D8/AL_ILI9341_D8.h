/*
 * Copyright Anders Liu.
 * 
 * 2.8" TFT based on ILI9341 (240x320) with a single port for 8-bit data interface.
 *
 * For most 2.8 TFT on market, the data lines are separated into 2 ports and hard to program, and potentially low performance.
 * So with this library, you need re-arrange pins with customized adapter pcb or flat cable.
 *
 * For more information, visit https://github.com/anders-liu/arduino-libs/tree/master/AL_ILI9341_D8
 */

#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "AL_GraphScreen.h"

// For some module, touch screen shares pins with TFT screen.
// So need to recover pin mode before transfer data.
// If your module doesn't have touch screen, comment out the following definition.
#define RECOVER_PIN_BEFORE_TRANSFER

//
// Fonts
//
// If you want to utilize text drawing, please define one and only one font before include this header.
// If more than one font defined, the priority is as the following list, first come first serve.
// #define USE_FONT_5X7    /* 5x7 ASCII font */
// #define USE_FONT_ASC12  /* 8x12 ASCII font */
// #define USE_FONT_ASC16  /* 8x16 ASCII font */

//#define USE_FONT_5X7
//#define USE_FONT_ASC12
#define USE_FONT_ASC16

#if defined(USE_FONT_5X7)

#define SUPPORT_DRAW_TEXT
#define TXT_CHAR_W 5
#define TXT_CHAR_H 7
#define CHAR_PATTERN_BYTES 5

#elif defined(USE_FONT_ASC12)

#define SUPPORT_DRAW_TEXT
#define TXT_CHAR_W 8
#define TXT_CHAR_H 12
#define CHAR_PATTERN_BYTES 12

#elif defined(USE_FONT_ASC16)

#define SUPPORT_DRAW_TEXT
#define TXT_CHAR_W 8
#define TXT_CHAR_H 16
#define CHAR_PATTERN_BYTES 16
#endif

#define LONG_SIDE 320
#define SHORT_SIDE 240

class AL_ILI9341_D8
#ifdef SUPPORT_DRAW_TEXT
    : public AL_GraphScreen
#else
    : public AL_GraphScreenWithText
#endif
{
  public:
    /**
     * Construct an ILI9341 LCD object.
     *
     * @param pPort Pointer to the PORTx that connected to LCD data lines.
     * @param pDdr Pointer to DDRx register corresonding to the data port.
     */
    AL_ILI9341_D8(
        volatile uint8_t *pPort, volatile uint8_t *pPin, volatile uint8_t *pDdr,
        byte rstPin, byte csPin, byte rsPin, byte wrPin, byte rdPin,
        AL_ScreenOrientation orientation = AL_SO_PORTRAIT1)
        : AL_GraphScreen(LONG_SIDE, SHORT_SIDE, orientation),
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
     * @returns 0x00009341.
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

    void changeOrientation(AL_ScreenOrientation o);

    void fillRect(
        uint16_t x, uint16_t y,
        uint16_t w, uint16_t h,
        AL_RgbColor color);

#ifdef SUPPORT_DRAW_TEXT
    void drawText(
        uint16_t x, uint16_t y,
        AL_RgbColor foreColor, AL_RgbColor backColor,
        const char *str, uint8_t maxLen = 100);
#endif

  private:
    uint16_t color565(AL_RgbColor color);
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
