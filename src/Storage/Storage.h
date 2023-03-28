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

	void setSpeed(uint16_t speed);
	[[nodiscard]] uint16_t getSpeed() const;

private:
	void readProgs();
	void writeProgs();

	Slot slots[4] = {};
	bool saved[4] = { false };
	static constexpr const char* path = "/Slots.bin";

	static constexpr uint16_t SpeedMin = 500;
	static constexpr uint16_t SpeedMax = 2000;
	static constexpr uint16_t SpeedDefault = 1000;
	uint16_t speed = SpeedDefault;
};

extern Storage storage;
#endif //ARMSTRONG_FIRMWARE_STORAGE_H
