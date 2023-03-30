#include "ColorService.h"
#include <Armstrong.h>
#include <Loop/LoopManager.h>

ColorService::ColorService(){
	LoopManager::addListener(this);
}

ColorService::~ColorService(){
	LoopManager::removeListener(this);
}

void ColorService::loop(uint micros){
	RGB.set(Sensor.getPixel());
}