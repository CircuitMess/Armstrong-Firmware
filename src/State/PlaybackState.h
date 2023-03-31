#ifndef ARMSTRONG_FIRMWARE_PLAYBACKSTATE_H
#define ARMSTRONG_FIRMWARE_PLAYBACKSTATE_H

#include <Input/InputListener.h>
#include <EncoderInput.h>
#include "State.h"
#include <Loop/LoopListener.h>

class PlaybackState : public State, private InputListener, private EncoderListener, private LoopListener {
public:
	PlaybackState();

protected:
	void onStart() override;
	void onStop() override;

private:
	void encoderMove(uint8_t enc, int8_t amount) override;
	void buttonPressed(uint i) override;
	void loop(uint micros) override;

	Input* input;

	uint32_t stepTimer = 0;
	uint8_t currentStep = 0;
	uint8_t savedSteps = 0;

	static constexpr uint8_t RotationMultiplier = 4;
};


#endif //ARMSTRONG_FIRMWARE_PLAYBACKSTATE_H
