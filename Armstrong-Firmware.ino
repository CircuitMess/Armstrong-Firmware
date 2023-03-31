#include <Arduino.h>
#include <CircuitOS.h>
#include <Armstrong.h>
#include <SPIFFS.h>
#include <Loop/LoopManager.h>
#include "src/State/RecordState.h"
#include "src/State/I2CState.h"
#include "src/Storage/Storage.h"
#include "src/ColorService.h"
#include "src/JigHWTest.h"

bool checkWheelsonConnected(){
	int sum = 0;
	for(int i = 0; i < 5; i++){
		sum += analogRead(35);
	}
	return (sum / 5.0) > 900;
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