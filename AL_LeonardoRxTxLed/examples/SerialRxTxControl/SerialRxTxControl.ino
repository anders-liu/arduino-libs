#include <Arduino.h>
#include "AL_LeonardoRxTxLed.h"

AL_LeonardoRxTxLed rxtx;

void setup() {
	Serial.begin(115200);
}

void loop() {
	if (Serial.available()) {
		String command = Serial.readString();
		if (command == "rxon") {
			rxtx.rxOn();
		}
		else if (command == "rxoff") {
			rxtx.rxOff();
		}
		else if (command == "txon") {
			rxtx.txOn();
		}
		else if (command == "txoff") {
			rxtx.txOff();
		}
	}
}