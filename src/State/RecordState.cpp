#include "RecordState.h"
#include <Armstrong.h>
#include "../Storage/Storage.h"
#include "PlaybackState.h"

RecordState::RecordState() : input(Armstrong.getInput()){}

void RecordState::onStart(){
	LED.clearAll();
	for(int i = 0; i < 4; ++i){
		LED.set(i, storage.isSaved(i));
	}
	LED.set(LED_PP, false); //PP LED is off in record

	input->addListener(this);
	for(const auto& btn: { BTN_1, BTN_2, BTN_3, BTN_4, BTN_PP }){
		setButtonHoldTime(btn, HoldMillis);
	}
	Encoder.addListener(this);
}

void RecordState::onStop(){
	LED.clearAll();
	input->removeListener(this);
	Encoder.removeListener(this);
}

void RecordState::encoderMove(uint8_t enc, int8_t amount){
	int val = Servo.getPos(enc) + amount * RotationMultiplier;
	Servo.setPos(enc, constrain(val, 0, 255));
}

void RecordState::buttonReleased(uint i){
	if(i == BTN_PP) return;

	auto index = buttonToIndex(i);
	if(input->getButtonHeldMillis(i) >= HoldMillis) return;

	if(!storage.isSaved(buttonToIndex(i))) return;

	auto slot = storage.getSlot(index);
	for(int j = 0; j < 4; ++j){
		Servo.setPos(j, slot.servoData[j]);
	}
}

void RecordState::buttonHeld(uint i){
	if(i == BTN_PP){
		stop();
		(new PlaybackState())->start();
	}else{
		auto index = buttonToIndex(i);
		if(storage.isSaved(index)){
			storage.clearSlot(index);
			LED.set(index, false);
		}else{
			storage.setSlot(index, { Servo.getPos(0), Servo.getPos(1), Servo.getPos(2), Servo.getPos(3) });
			LED.set(index, true);
		}
	}
}

uint8_t RecordState::buttonToIndex(uint8_t btn) const{
	switch(btn){
		case BTN_1:
			return 0;
		case BTN_2:
			return 1;
		case BTN_3:
			return 2;
		case BTN_4:
			return 3;
		default:
			return 0;
	}
}


