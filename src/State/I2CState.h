#ifndef ARMSTRONG_FIRMWARE_I2CSTATE_H
#define ARMSTRONG_FIRMWARE_I2CSTATE_H

#include <Loop/LoopListener.h>
#include "State.h"

class I2CState : public State, private LoopListener {
protected:
	void onStart() override;
	void onStop() override;

private:
	void loop(uint micros) override;

	static constexpr uint8_t ReadByte = 0x77;
};


#endif //ARMSTRONG_FIRMWARE_I2CSTATE_H