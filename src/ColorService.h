#ifndef ARMSTRONG_FIRMWARE_COLORSERVICE_H
#define ARMSTRONG_FIRMWARE_COLORSERVICE_H

#include <Loop/LoopListener.h>

class ColorService : private LoopListener {
public:
	ColorService();
	~ColorService() override;
private:
	void loop(uint micros) override;
};


#endif //ARMSTRONG_FIRMWARE_COLORSERVICE_H
