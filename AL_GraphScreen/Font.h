//
// Copyright Anders Liu.
//
// Font information.
//
// For more information, visit https://github.com/anders-liu/arduino-libs/tree/master/AL_GraphScreen
//

#ifndef FONT__H
#define FONT__H

#include "AL_GraphScreen_Config.h"

// Check is any font enabled.
#if defined(AL_GS_ENABLE_FONT_5X7) || defined(AL_GS_ENABLE_FONT_8X12) || defined(AL_GS_ENABLE_FONT_8X16)
#define TEXT_ENABLED
#endif

// Check is only one font enabled.
#if defined(AL_GS_ENABLE_FONT_5X7) && !defined(AL_GS_ENABLE_FONT_8X12) && !defined(AL_GS_ENABLE_FONT_8X16)
#define ONLY_ONE_FONT
#define ONLY_ONE_FONT_DATA font5x7Data
#define ONLY_ONE_FONT_BYTES_PER_CHAR FONT_BYTES_PER_CHAR_5X7
#define ONLY_ONE_FONT_CHAR_W CHAR_W_5X7
#define ONLY_ONE_FONT_CHAR_H CHAR_H_5X7
#endif
#if !defined(AL_GS_ENABLE_FONT_5X7) && defined(AL_GS_ENABLE_FONT_8X12) && !defined(AL_GS_ENABLE_FONT_8X16)
#define ONLY_ONE_FONT
#define ONLY_ONE_FONT_DATA font8x12Data
#define ONLY_ONE_FONT_BYTES_PER_CHAR FONT_BYTES_PER_CHAR_8X12
#define ONLY_ONE_FONT_CHAR_W CHAR_W_8X12
#define ONLY_ONE_FONT_CHAR_H CHAR_H_8X12
#endif
#if !defined(AL_GS_ENABLE_FONT_5X7) && !defined(AL_GS_ENABLE_FONT_8X12) && defined(AL_GS_ENABLE_FONT_8X16)
#define ONLY_ONE_FONT
#define ONLY_ONE_FONT_DATA font8x16Data
#define ONLY_ONE_FONT_BYTES_PER_CHAR FONT_BYTES_PER_CHAR_8X16
#define ONLY_ONE_FONT_CHAR_W CHAR_W_8X16
#define ONLY_ONE_FONT_CHAR_H CHAR_H_8X16
#endif

#include <avr/pgmspace.h>

#ifdef AL_GS_ENABLE_FONT_5X7
extern PGM_P font5x7Data;
#define FONT_BYTES_PER_CHAR_5X7 5
#define CHAR_W_5X7 5
#define CHAR_H_5X7 7
#endif // AL_GS_ENABLE_FONT_5X7

#ifdef AL_GS_ENABLE_FONT_8X12
extern PGM_P font8x12Data;
#define FONT_BYTES_PER_CHAR_8X12 12
#define CHAR_W_8X12 8
#define CHAR_H_8X12 12
#endif // AL_GS_ENABLE_FONT_8X12

#ifdef AL_GS_ENABLE_FONT_8X16
extern PGM_P font8x16Data;
#define FONT_BYTES_PER_CHAR_8X16 16
#define CHAR_W_8X16 8
#define CHAR_H_8X16 16
#endif // AL_GS_ENABLE_FONT_8X16

#endif // FONT__H
