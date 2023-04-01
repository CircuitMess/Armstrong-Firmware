#ifndef ARMSTRONG_FIRMWARE_PLAYBACKSTATE_H
#define ARMSTRONG_FIRMWARE_PLAYBACKSTATE_H

#include <Input/InputListener.h>
#include <EncoderInput.h>
#include "../State.h"
#include "../SlotStorage.h"
#include <Loop/LoopListener.h>

class PlaybackState : public State, private InputListener, private EncoderListener, private LoopListener {
protected:
	void onStart() override;
	void onStop() override;

private:
	void encoderMove(Motor enc, int8_t amount) override;
	void buttonPressed(uint i) override;
	void loop(uint micros) override;

	void positionCurrent();

	int32_t stepTime = 1000000; // [us]
	static constexpr uint32_t MinStepTime = 500000;
	static constexpr uint32_t MaxStepTime = 2000000;
	static constexpr uint32_t StepTimeStep = 50000;

	uint32_t stepTimer = 0;
	uint8_t currentStep = 0;

	struct Step {
		Slot slot;
		ServoPositions motors;
	};

	std::vector<Step> steps;

};


#endif //ARMSTRONG_FIRMWARE_PLAYBACKSTATE_H
