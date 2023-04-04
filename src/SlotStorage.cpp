#include "SlotStorage.h"
#include <SPIFFS.h>

SlotStorage Storage;

ServoPositions SlotStorage::getSlot(uint8_t index) const{
	if(index >= 4) return {};
	return slots[index];
}

void SlotStorage::setSlot(uint8_t index, const ServoPositions& positions){
	if(index >= 4) return;
	slots[index] = positions;
}

void SlotStorage::clearSlot(uint8_t index){
	if(index >= 4) return;
	slots[index].clear();
}

void SlotStorage::clearAll(){
	for(int i = 0; i < 4; ++i){
		clearSlot(i);
	}
}

void SlotStorage::load(){
	clearAll();

	File f = SPIFFS.open(path, "r");
	if(!f || f.size() == 0) return;

	while(f.available()){
		uint8_t slotIndex;
		f.read(&slotIndex, 1);

		for(int j = 0; j < 4; j++){
			auto motor = (Motor) j;

			uint8_t val;
			f.read(&val, 1);

			slots[slotIndex].insert({ motor, val });
		}
	}
}

void SlotStorage::save(){
	SPIFFS.remove(path);
	File f = SPIFFS.open(path, "w");

	for(uint8_t i = 0; i < 4; i++){
		if(slots[i].empty()) continue;
		f.write(&i, 1);

		for(int j = 0; j < 4; j++){
			auto motor = (Motor) j;
			auto val = slots[i].find(motor)->second;

			f.write(&val, 1);
		}
	}
}
