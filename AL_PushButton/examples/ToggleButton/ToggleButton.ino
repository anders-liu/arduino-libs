#include "AL_PushButton.h"

#define LED_PIN 4
#define BUTTON_PIN 5

AL_PushButton btn(BUTTON_PIN);
byte led = 0;

void onKeyDown()
{
    led = !led;
    digitalWrite(LED_PIN, led);
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    btn.handleKeyDown(onKeyDown);
    btn.setup();
}

void loop()
{
    btn.loop();
}