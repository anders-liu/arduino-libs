#include <Wire.h>
#include "AL_PCA9685.h"

#define PWM_ADDR 0x40
#define PWM_OE_PIN 12

AL_PCA9685 pwm(PWM_ADDR, PWM_OE_PIN);

void setup()
{
    Wire.begin();

    pwm.setup();
    pwm.setOutputEnable(true);

    for (uint8_t channel = 0; channel < 16; channel++)
    {
        uint16_t offTime = 256 * channel;
        pwm.setOffTime(channel, offTime);
    }
}

void loop()
{
}