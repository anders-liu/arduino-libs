# AL_ILI9341_D8.h Customization

## Pin sharing and recovery

For some ILI9341 based TFT module, touch screen shares pins with TFT screen.
So you have to recover pin mode before transfer data.
In this case, add or uncomment the following definition to AL_ILI9341_D8.h:

```C++
#define RECOVER_PIN_BEFORE_TRANSFER
```

## Using drawText and font

If you want to use ```drawText``` function, you have to define a constant to indicate which font do you want to use.

*NOTE*, for save your program space, only one font can be used in a signle program.

All supported fonts are dot-array font, and the patterns are burned into your program space (PROGMEM).

For using standard 5x7 ASCII font (5 bytes per char), define:

```C++
#define USE_FONT_5X7
```

For using 8x12 font (12 bytes per char), define:

```C++
#define USE_FONT_ASC12
```

For using 8x16 font (16 bytes per char), define:

```C++
#define USE_FONT_ASC16
```
