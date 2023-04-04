#ifndef ARMSTRONG_FIRMWARE_JIGHWTEST_H
#define ARMSTRONG_FIRMWARE_JIGHWTEST_H

#include <Display/Display.h>

class JigHWTest {
public:
	JigHWTest();
	void start();

private:
	void AudioVisualTest();

	const uint32_t FlashTime = 1000;
	uint32_t flashTimer = 0;
	bool show = false;
};

#endif //ARMSTRONG_FIRMWARE_JIGHWTEST_H
