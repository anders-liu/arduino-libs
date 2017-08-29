#include "AL_RotrayEncoder.h"
#include "AL_PushButton.h"

#define ENC_A_PIN 49
#define ENC_B_PIN 48
#define ENC_SW_PIN 47

AL_RotrayEncoder enc(ENC_A_PIN, ENC_B_PIN);
AL_PushButton sw(ENC_SW_PIN);

int32_t counter = 0;

void onClockwiseTick()
{
    counter++;
    Serial.print("onClockwiseTick: ");
    Serial.println(counter);
}

void onCounterclockwiseTick()
{
    counter--;
    Serial.print("onCounterclockwiseTick: ");
    Serial.println(counter);
}

void onSwKeyDown()
{
    counter = 0;
    Serial.print("onSwKeyDown: ");
    Serial.println(counter);
}

void setup()
{
    Serial.begin(9600);

    enc.setup();
    enc.handleClockwiseTick(onClockwiseTick);
    enc.handleCounterclockwiseTick(onCounterclockwiseTick);

    sw.setup();
    sw.handleKeyDown(onSwKeyDown);
}

void loop()
{
    enc.loop();
    sw.loop();
}
