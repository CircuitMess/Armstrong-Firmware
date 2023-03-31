#include <Arduino.h>
#include <CircuitOS.h>
#include <Armstrong.h>
#include <SPIFFS.h>
#include <Loop/LoopManager.h>
#include "src/State/RecordState.h"
#include "src/Storage/Storage.h"

void setup(){
	Serial.begin(115200);
	Armstrong.begin();
	if(!SPIFFS.begin()){
		Serial.println("SPIFFS begin error");
		return;
	}

	storage.begin();

	(new RecordState())->start();
}

void loop(){
	LoopManager::loop();
}