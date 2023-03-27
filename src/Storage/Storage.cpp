#include "Storage.h"
#include <SPIFFS.h>

Storage storage;

void Storage::begin(){
	if(SPIFFS.exists(path)){
		readProgs();
	}else{
		writeProgs();
	}
}

bool Storage::isSaved(uint8_t index) const{
	if(index >= 4) return false;
	return saved[index];
}

Slot Storage::getSlot(uint8_t index) const{
	if(index >= 4) return {};
	if(!saved[index]) return {};
	return slots[index];
}

void Storage::setSlot(uint8_t index, const Slot& program){
	if(index >= 4) return;
	slots[index] = program;
	saved[index] = true;
	writeProgs();
}

void Storage::clearSlot(uint8_t index){
	if(index >= 4) return;
	saved[index] = false;
	writeProgs();
}

void Storage::clearAll(){
	for(int i = 0; i < 4; ++i){
		clearSlot(i);
	}
}

void Storage::readProgs(){
	File f = SPIFFS.open(path, "r");
	if(!f) return;
	for(int i = 0; i < 4; ++i){
		f.read((uint8_t*) &saved[i], 1);
		f.read((uint8_t*) &slots[i], 4);
	}
	f.close();
}

void Storage::writeProgs(){
	File f = SPIFFS.open(path, "w");
	for(int i = 0; i < 4; ++i){
		f.write(saved[i]);
		f.write((uint8_t*) &slots[i], sizeof(Slot));
	}
	f.close();
}
