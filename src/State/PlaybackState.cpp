#include "PlaybackState.h"
#include "RecordState.h"
#include <Armstrong.h>
#include "../Storage/Storage.h"
#include <Loop/LoopManager.h>

PlaybackState::PlaybackState() : input(Armstrong.getInput()){}

void PlaybackState::onStart(){
	input->addListener(this);
	Encoder.addListener(this);

	currentStep = 0;
	stepTimer = 0;
	savedSteps = 0;
	for(uint8_t i = 0; i < 4; i++){
		if(storage.isSaved(i)) savedSteps++;
	}
	if(savedSteps == 0) return;

	while(!storage.isSaved(currentStep)){
		currentStep = (currentStep + 1) % 4;
	}

	auto slot = storage.getSlot(currentStep);
	for(int j = 0; j < 4; ++j){
		Servo.setPos(j, slot.servoData[j]);
	}
	LED.clearAll();
	LED.set(LED_PP, true);
	LED.set(currentStep, true);

	if(savedSteps == 1) return;

	LoopManager::addListener(this);

}

void PlaybackState::onStop(){
	LED.clearAll();
	input->removeListener(this);
	Encoder.removeListener(this);
	LoopManager::removeListener(this);
}

void PlaybackState::encoderMove(uint8_t enc, int8_t amount){
	int speed = storage.getSpeed() + amount * RotationMultiplier;
	storage.setSpeed(speed);
}

void PlaybackState::buttonPressed(uint i){
	if(i == BTN_PP){
		stop();
		(new RecordState())->start();
	}
}

void PlaybackState::loop(uint micros){
	stepTimer += micros;
	if(stepTimer >= storage.getSpeed() * 1000){
		stepTimer = 0;
		LED.set(currentStep, false);
		do {
			currentStep = (currentStep + 1) % 4;
		}while(!storage.isSaved(currentStep));

		LED.set(currentStep, true);

		auto slot = storage.getSlot(currentStep);
		for(int j = 0; j < 4; ++j){
			Servo.setPos(j, slot.servoData[j]);
		}
	}
}

