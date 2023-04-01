#include "PlaybackState.h"
#include "RecordState.h"
#include <Armstrong.h>
#include <Loop/LoopManager.h>

void PlaybackState::onStart(){
	steps.clear();

	for(int i = 0; i < 4; i++){
		auto data = Storage.getSlot(i);
		if(data.empty()) continue;

		Step step = { (Slot) i, data };

		steps.push_back(step);
		LED.set(step.slot, true);
	}

	if(steps.empty()){
		LoopManager::defer([](uint32_t){
			enterRecord();
		});

		return;
	}

	LED.set(Slot::PlayPause, true);

	positionCurrent();
	stepTimer = 0;

	Input::getInstance()->addListener(this);
	Encoders.addListener(this);
	LoopManager::addListener(this);
}

void PlaybackState::onStop(){
	LED.clear();

	Input::getInstance()->removeListener(this);
	Encoders.removeListener(this);
	LoopManager::removeListener(this);
}

void PlaybackState::encoderMove(Motor enc, int8_t amount){
	stepTime -= amount*StepTimeStep;
	stepTime = constrain(stepTime, MinStepTime, MaxStepTime);
}

void PlaybackState::buttonPressed(uint i){
	if(i != BTN_PP) return;

	LoopManager::defer([](uint32_t){
		enterRecord();
	});
}

void PlaybackState::loop(uint micros){
	stepTimer += micros;
	if(stepTimer <= stepTime) return;
	stepTimer = 0;

	currentStep = (currentStep+1) % steps.size();
	positionCurrent();
}

void PlaybackState::positionCurrent(){
	const auto& step = steps[currentStep];

	for(const auto& motor : step.motors){
		Motors.setPos(motor.first, motor.second);
	}

	LED.set(step.slot, false);
	delay(100);
	LED.set(step.slot, true);
}
