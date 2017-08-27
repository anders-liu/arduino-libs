#include <Wire.h>
#include "AL_TCS3472.h"

#define CS_ADDR 0x29
#define CS_LED_PIN 13

AL_TCS3472 cs(CS_ADDR, CS_LED_PIN);

void setup()
{
    Serial.begin(9600);

    Wire.begin();
    cs.setup();
    cs.powerOn(true);
}

void loop()
{
    AL_RgbColor color = cs.read();
    float hue = color.getHue();

    Serial.print("R: ");
    Serial.print(color.r);
    Serial.print(", G: ");
    Serial.print(color.g);
    Serial.print(", B: ");
    Serial.print(color.b);
    Serial.print(", Hue: ");
    Serial.print(hue);
    Serial.println();

    delay(500);
}