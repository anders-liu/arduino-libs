#include <Arduino.h>
#include "AL_LeonardoRxTxLed.h"

AL_LeonardoRxTxLed rxtx;

byte num = 0;

void setup() {
	rxtx.rxOff();
	rxtx.txOff();
}

void loop() {
	delay(500);

	if (num & 0x01) {
		rxtx.txOn();
	}
	else {
		rxtx.txOff();
	}

	if (num & 0x02) {
		rxtx.rxOn();
	}
	else {
		rxtx.rxOff();
	}

	num++;
}