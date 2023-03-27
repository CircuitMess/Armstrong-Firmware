#ifndef ARMSTRONG_FIRMWARE_STORAGE_H
#define ARMSTRONG_FIRMWARE_STORAGE_H

#include <Arduino.h>

struct Slot {
	uint8_t servoData[4];
};

class Storage {
public:
	void begin();

	[[nodiscard]] bool isSaved(uint8_t index) const;
	[[nodiscard]] Slot getSlot(uint8_t index) const;
	void setSlot(uint8_t index, const Slot& program);
	void clearSlot(uint8_t index);
	void clearAll();

private:
	void readProgs();
	void writeProgs();

	Slot slots[4] = {};
	bool saved[4] = { false };
	static constexpr const char* path = "/Slots.bin";
};

extern Storage storage;
#endif //ARMSTRONG_FIRMWARE_STORAGE_H
