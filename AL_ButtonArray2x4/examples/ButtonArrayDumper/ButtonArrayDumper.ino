#include "AL_ButtonArray2x4.h"

#define COL0 3
#define COL1 2
#define COL2 5
#define COL3 9
#define ROW0 A1
#define ROW1 4

AL_ButtonArray2x4 ba(COL0, COL1, COL2, COL3, ROW0, ROW1);

void onKeyDown(uint8_t /* AL_BA2X4_KEYxx */ key);
void onKeyUp(uint8_t /* AL_BA2X4_KEYxx */ key);

void setup()
{
    Serial.begin(115200);
    ba.handleKeyDown(onKeyDown);
    ba.handleKeyUp(onKeyUp);
    ba.setup();
}

void loop()
{
    ba.loop();
}

void onKeyDown(uint8_t /* AL_BA2X4_KEYxx */ key)
{
    Serial.println("OnKeyDown: ");
    Serial.println(key);
}

void onKeyUp(uint8_t /* AL_BA2X4_KEYxx */ key)
{
    Serial.println("OnKeyUp: ");
    Serial.println(key);
}
