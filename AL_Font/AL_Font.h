//
// AL_Font.h
//
// Define ASCII 8x16 font.
//
// Copyright Anders Liu.
//
// For more information, visit https://github.com/anders-liu/arduino-libs
//

#ifndef AL_FONT__H
#define AL_FONT__H

#include <Arduino.h>
#include <stdint.h>

#define AL_FONT_BYTES_PER_CHAR 16
#define AL_FONT_CHAR_W 8
#define AL_FONT_CHAR_H 16

extern const uint8_t AL_FontData[];

#endif // AL_FONT__H
