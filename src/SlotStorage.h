#ifndef ARMSTRONG_FIRMWARE_SLOTSTORAGE_H
#define ARMSTRONG_FIRMWARE_SLOTSTORAGE_H

#include <Arduino.h>
#include <Names.h>
#include <unordered_map>

typedef std::unordered_map<Motor, uint8_t, MotorHash> ServoPositions;

class SlotStorage {
public:
	ServoPositions getSlot(uint8_t index) const;
	void setSlot(uint8_t index, const ServoPositions& positions);
	void clearSlot(uint8_t index);
	void clearAll();

	void load();
	void save();

private:

	ServoPositions slots[4] = {};

	static constexpr const char* path = "/Slots.bin";

};

extern SlotStorage Storage;

#endif //ARMSTRONG_FIRMWARE_SLOTSTORAGE_H
