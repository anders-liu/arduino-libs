/*
 * Copyright Anders Liu.
 * 
 * A push button library with anti-vibration, no delay.
 *
 * For more information, visit https://github.com/anders-liu/arduino-libs/tree/master/AL_PushButton
 */

#pragma once

#include <Arduino.h>

typedef void (*AL_PushButtonEventHandler)(void);

/**
 * Indicates a push button.
 */
class AL_PushButton
{
  public:
    /**
     * Construct a push button with the specified pin.
     *
     * @param pin The pin that this push button connected.
     * This pin will always be set as INPUT_PULLUP.
     */
    AL_PushButton(byte pin)
        : pin(pin),
          state(0xF0),
          isPushedValue(false),
          lastMillis(0),
          keyDownHandler(nullptr),
          keyUpHandler(nullptr)
    {
    }

    /**
     * Should be called in sketch's setup() function, initialize push button.
     */
    void setup();

    /**
     * Should be called in sketch's loop() function, check push button state and fire events if needed.
     */
    void loop();

    /**
     * Set KeyDown event handler.
     */
    void handleKeyDown(AL_PushButtonEventHandler handler) { keyDownHandler = handler; }

    /**
     * Set KeyUp event handler.
     */
    void handleKeyUp(AL_PushButtonEventHandler handler) { keyUpHandler = handler; }

    bool isPushed() { return isPushedValue; }

  private:
    byte pin;
    byte state;
    bool isPushedValue;
    unsigned long lastMillis;
    AL_PushButtonEventHandler keyDownHandler;
    AL_PushButtonEventHandler keyUpHandler;

    void handleKeyDown();
    void handleKeyUp();
};