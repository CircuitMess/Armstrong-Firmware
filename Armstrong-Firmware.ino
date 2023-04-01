#include <Arduino.h>
#include <Armstrong.h>
#include <CircuitOS.h>
#include <Loop/LoopManager.h>

#include "src/ColorService.h"
#include "src/SlotStorage.h"
#include "src/State.h"

bool checkWheelson(){
	uint32_t sum = 0;
	const int count = 15;

	for(int i = 0; i < count; i++){
		sum += analogRead(PIN_WHEELSON);
	}

	return (sum / count) > 800;
}

void setup(){
	Serial.begin(115200);
	Armstrong.begin();

	new ColorService;

	if(checkWheelson()){
		State::enterI2C();
	}else{
		Storage.load();
		State::enterRecord();
	}
}

void loop(){
	LoopManager::loop();
}