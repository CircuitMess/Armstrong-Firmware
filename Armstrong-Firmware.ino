#include <Arduino.h>
#include <CircuitOS.h>
#include <Armstrong.h>
#include <SPIFFS.h>
#include <Loop/LoopManager.h>
#include "src/State/RecordState.h"
#include "src/State/I2CState.h"
#include "src/Storage/Storage.h"
#include "src/ColorService.h"

bool checkWheelsonConnected(){
	int sum = 0;
	for(int i = 0; i < 5; i++){
		sum += analogRead(35);
	}
	return (sum / 5.0) > 900;
}

void setup(){
	Serial.begin(115200);
	Armstrong.begin();
	if(!SPIFFS.begin()){
		Serial.println("SPIFFS begin error");
		return;
	}

	storage.begin();

	new ColorService;

	if(checkWheelsonConnected()){
		(new I2CState())->start();

	}else{
		(new RecordState())->start();
	}
}

void loop(){
	LoopManager::loop();
}