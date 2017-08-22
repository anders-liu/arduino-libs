//
// AL_GraphScreen_Config.h
//
// Provides configurations for AL_GraphScreen library.
//

#ifndef AL_GRAPHSCREEN_CONFIG__H
#define AL_GRAPHSCREEN_CONFIG__H

//--------------------------------------------------------------
// Wiring configurations.
//--------------------------------------------------------------

// For some module, touch screen shares pins with TFT screen.
// So need to recover pin mode before transfer data.
// If your module doesn't have touch screen, comment out the following definition.
#define AL_GS_RECOVER_PIN_BEFORE_TRANSFER

//--------------------------------------------------------------
// Screen configurations.
//--------------------------------------------------------------

// To enable ILI9341 based TFT screen, uncomment the following line:
//#define AL_GS_ENABLE_ILI9341

// To enable ILI9481/ILI9488 based TFT screen, uncomment the following definition:
#define AL_GS_ENABLE_ILI9481

//--------------------------------------------------------------
// Text configurations.
//--------------------------------------------------------------

// To enable 5x7 ASCII font, uncomment the following definition:
// This will cost 1,280 bytes (5x256) in program space.
//#define AL_GS_ENABLE_FONT_5X7

// To enable 8x12 ASCII font, uncomment the following definition:
// This will cost 3,072 bytes (12x256) in program space.
//#define AL_GS_ENABLE_FONT_8X12

// To enable 8x16 ASCII font, uncomment the following definition:
// This will cost 4,096 bytes (16x256) in program space.
#define AL_GS_ENABLE_FONT_8X16

#endif // AL_GRAPHSCREEN_CONFIG__H
