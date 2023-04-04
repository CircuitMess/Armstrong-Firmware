#include <Loop/LoopManager.h>
#include "State.h"
#include "States/RecordState.h"
#include "States/PlaybackState.h"
#include "States/I2CState.h"

std::unique_ptr<State> State::currentState = nullptr;

void State::start(){
	if(started) return;
	started = true;
	onStart();
}

void State::stop(){
	if(!started) return;
	started = false;
	onStop();
}

bool State::isStarted() const{
	return started;
}

void State::onStart(){

}

void State::onStop(){

}

void State::clearCurrent(){
	if(!currentState) return;
	currentState->stop();
	currentState.reset();
}

void State::startNew(State* state){
	currentState.reset(state);

	LoopManager::defer([state](uint32_t){
		state->start();
	});
}

void State::enterRecord(){
	clearCurrent();
	startNew(new RecordState);
}

void State::enterPlayback(){
	clearCurrent();
	startNew(new PlaybackState);
}

void State::enterI2C(){
	clearCurrent();
	startNew(new I2CState);
}
