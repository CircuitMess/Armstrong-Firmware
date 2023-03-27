#include <Arduino.h>
#include <Armstrong.h>
#include <SPIFFS.h>
#include "src/State/RecordState.h"
#include "src/Storage/Storage.h"

void setup(){
	Armstrong.begin();
	if(!SPIFFS.begin()){
		Serial.println("SPIFFS begin error");
	}

	storage.begin();

	(new RecordState())->start();
}

void loop(){

}