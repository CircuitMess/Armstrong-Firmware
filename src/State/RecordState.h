#ifndef ARMSTRONG_FIRMWARE_RECORDSTATE_H
#define ARMSTRONG_FIRMWARE_RECORDSTATE_H

#include <Input/InputListener.h>
#include <EncoderInput.h>
#include "State.h"

class RecordState : public State, private InputListener, private EncoderListener {
public:
	RecordState();
protected:
	void onStart() override;
	void onStop() override;

private:
	void buttonReleased(uint i) override;
	void buttonHeld(uint i) override;
	void encoderMove(uint8_t enc, int8_t amount) override;

	Input* input;

	static constexpr uint32_t HoldMillis = 850;
	static constexpr uint8_t RotationMultiplier = 2;

	uint8_t buttonToIndex(uint8_t btn) const;
};


#endif //ARMSTRONG_FIRMWARE_RECORDSTATE_H
