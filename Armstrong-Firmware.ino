#include <Arduino.h>
#include <Armstrong.h>
#include <CircuitOS.h>
#include <Loop/LoopManager.h>

#include "src/ColorService.h"
#include "src/SlotStorage.h"
#include "src/State.h"
#include "src/JigHWTest.h"


bool checkWheelson(){
	uint32_t sum = 0;
	const int count = 15;

	for(int i = 0; i < count; i++){
		sum += analogRead(PIN_WHEELSON);
	}

	return (sum / count) > 800;
}

bool checkJig(){
	char buf[7];
	int wp = 0;

	uint32_t start = millis();
	while(millis() - start < 500){
		while(Serial.available()){
			buf[wp] = Serial.read();
			wp = (wp + 1) % 7;

			for(int i = 0; i < 7; i++){
				int match = 0;
				static const char* target = "JIGTEST";

				for(int j = 0; j < 7; j++){
					match += buf[(i + j) % 7] == target[j];
				}

				if(match == 7) return true;
			}
		}
	}

	return false;
}


void setup(){
	Serial.begin(115200);

	if(checkJig()){
		printf("Jig\n");
		auto test = new JigHWTest();
		test->start();
		for(;;);
	}

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