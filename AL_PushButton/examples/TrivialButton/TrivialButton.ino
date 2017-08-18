#include "AL_PushButton.h"

#define LED_PIN LED_BUILTIN
#define BUTTON_PIN A11

AL_PushButton btn(BUTTON_PIN);

void onKeyDown()
{
    Serial.println("onKeyDown");
    digitalWrite(LED_PIN, HIGH);
}

void onKeyUp()
{
    Serial.println("onKeyUp");
    digitalWrite(LED_PIN, LOW);
}

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    btn.handleKeyDown(onKeyDown);
    btn.handleKeyUp(onKeyUp);
    btn.setup();
}

void loop()
{
    btn.loop();
}