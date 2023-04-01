#include "RecordState.h"
#include <Armstrong.h>
#include <Loop/LoopManager.h>
#include "PlaybackState.h"
#include "../SlotStorage.h"

RecordState::RecordState(){
	for(int i = 0; i < 8; i++){
		setButtonHoldTime(i, HoldMillis);
	}
}

void RecordState::onStart(){
	for(int i = 0; i < 4; i++){
		auto slot = Storage.getSlot(i);
		if(slot.empty()) continue;

		LED.set((Slot) i, true);
	}


	Input::getInstance()->addListener(this);
	Encoders.addListener(this);
}

void RecordState::onStop(){
	LED.clear();

	Input::getInstance()->removeListener(this);
	Encoders.removeListener(this);
}

void RecordState::encoderMove(Motor enc, int8_t amount){
	int val = Motors.getPos(enc);
	val = constrain(val + amount * RotationMultiplier, 0, 255);
	Motors.setPos(enc, val);
}

void RecordState::buttonPressed(uint btn){
	if(btn == BTN_PP) return;
	auto slot = (uint8_t) Armstrong.btnToSlot(btn);
	if(slot == None) return;

	slotHold[slot] = false;
}

void RecordState::buttonReleased(uint btn){
	if(btn == BTN_PP) return;

	auto slot = Armstrong.btnToSlot(btn);
	if(slot == None) return;

	auto slotIndex = (uint8_t) slot;

	if(slotHold[slotIndex]) return;

	auto slotData = Storage.getSlot(slotIndex);
	if(slotData.empty()) return;

	for(int i = 0; i < 4; i++){
		auto motor = (Motor) i;
		Motors.setPos(motor, slotData.find(motor)->second);
	}
}

void RecordState::buttonHeld(uint btn){
	if(btn == BTN_PP){
		bool hasSlot = false;
		for(int i = 0; i < 4; i++){
			auto data = Storage.getSlot(i);
			if(!data.empty()){
				hasSlot = true;
				break;
			}
		}

		if(!hasSlot) return;

		LoopManager::defer([](uint32_t){
			enterPlayback();
		});

		return;
	}

	auto slot = Armstrong.btnToSlot(btn);
	if(slot == None) return;

	auto slotIndex = (uint8_t) slot;

	auto slotData = Storage.getSlot(slotIndex);

	if(slotData.empty()){
		for(int i = 0; i < 4; i++){
			auto motor = (Motor) i;
			slotData.insert({ motor, Motors.getPos(motor) });
		}

		Storage.setSlot(slot, slotData);
		LED.set(slot, true);
	}else{
		Storage.clearSlot(slotIndex);
		LED.set(slot, false);
	}

	Storage.save();
	slotHold[slotIndex] = true;
}
