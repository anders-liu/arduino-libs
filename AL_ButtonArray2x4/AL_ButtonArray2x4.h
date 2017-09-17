//
// AL_ButtonArray2x4.h
//
// Provides functions for 2-row 4-column button array.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#ifndef AL_BUTTONARRAY2X4_H
#define AL_BUTTONARRAY2X4_H

#include <Arduino.h>

#define AL_BA2X4_KEY00 ((uint8_t)0)
#define AL_BA2X4_KEY01 ((uint8_t)1)
#define AL_BA2X4_KEY02 ((uint8_t)2)
#define AL_BA2X4_KEY03 ((uint8_t)3)
#define AL_BA2X4_KEY10 ((uint8_t)4)
#define AL_BA2X4_KEY11 ((uint8_t)5)
#define AL_BA2X4_KEY12 ((uint8_t)6)
#define AL_BA2X4_KEY13 ((uint8_t)7)

class AL_ButtonArray2x4
{
  public:
    AL_ButtonArray2x4(
        uint8_t pinCol0, uint8_t pinCol1, uint8_t pinCol2, uint8_t pinCol3,
        uint8_t pinRow0, uint8_t pinRow1)
        : pinCol0(pinCol0), pinCol1(pinCol1), pinCol2(pinCol2), pinCol3(pinCol3),
          pinRow0(pinRow0), pinRow1(pinRow1),
          lastKeys(0xFF), currKeys(0xFF), tempKeys(0xFF),
          tempHasBeenRead(false),
          lastMillis(0),
          keyDownHandler(nullptr), keyUpHandler(nullptr) {}

  public:
    typedef void (*KeyEventHandler)(uint8_t /* AL_BA2X4_KEYxx */ key);

  public:
    void setup();
    void loop();
    void handleKeyDown(KeyEventHandler handler) { keyDownHandler = handler; }
    void handleKeyUp(KeyEventHandler handler) { keyUpHandler = handler; }

  private:
    uint8_t readKeys();

  private:
    uint8_t pinCol0, pinCol1, pinCol2, pinCol3;
    uint8_t pinRow0, pinRow1;
    uint8_t lastKeys, currKeys, tempKeys;
    bool tempHasBeenRead;
    uint32_t lastMillis;
    KeyEventHandler keyDownHandler, keyUpHandler;
};

#endif // AL_BUTTONARRAY2X4_H
