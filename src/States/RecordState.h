#ifndef ARMSTRONG_FIRMWARE_RECORDSTATE_H
#define ARMSTRONG_FIRMWARE_RECORDSTATE_H

#include <Input/InputListener.h>
#include <EncoderInput.h>
#include "../State.h"

class RecordState : public State, private InputListener, private EncoderListener {
public:
	RecordState();

protected:
	void onStart() override;
	void onStop() override;

private:
	void buttonPressed(uint btn) override;
	void buttonReleased(uint btn) override;
	void buttonHeld(uint btn) override;
	bool slotHold[4] = {false};

	void encoderMove(Motor enc, int8_t amount) override;

	static constexpr uint32_t HoldMillis = 850;
	static constexpr uint8_t RotationMultiplier = 5;

};


#endif //ARMSTRONG_FIRMWARE_RECORDSTATE_H
