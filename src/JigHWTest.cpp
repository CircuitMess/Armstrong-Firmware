#include <SPIFFS.h>
#include "JigHWTest.h"
#include <Pins.hpp>
#include <Wire.h>
#include <ctime>
#include <Display/Display.h>

Display* display;
Sprite* canvas;

#define TFT_CS (-1)
#define TFT_SCL ENC_3A
#define TFT_SDA ENC_3B
#define TFT_DC ENC_4A
#define TFT_RST ENC_4B

lgfx::Panel_ST7735S* panel(){
	auto bus = new lgfx::Bus_SPI();
	auto panel = new lgfx::Panel_ST7735S();

	{
		auto cfg = bus->config();


		cfg.spi_host = HSPI_HOST;
		cfg.spi_mode = 0;
		cfg.freq_write = 27000000;
		cfg.freq_read = 27000000;
		cfg.spi_3wire = false;
		cfg.use_lock = true;
		cfg.dma_channel = 1;
		cfg.pin_sclk = TFT_SCL;
		cfg.pin_mosi = TFT_SDA;
		cfg.pin_miso = -1;
		cfg.pin_dc = TFT_DC;

		bus->config(cfg);
		panel->setBus(bus);
	}

	{
		auto cfg = panel->config();

		cfg.pin_cs = TFT_CS;
		cfg.pin_rst = TFT_RST;
		cfg.pin_busy = -1;


		cfg.memory_width = 128;
		cfg.memory_height = 160;
		cfg.panel_width = 128;
		cfg.panel_height = 160;
		cfg.offset_x = 0;
		cfg.offset_y = 0;
		cfg.offset_rotation = 0;
		cfg.readable = true;
		cfg.invert = false;
		cfg.rgb_order = false;
		cfg.dlen_16bit = false;
		cfg.bus_shared = true;


		panel->config(cfg);
	}

	return panel;
}


JigHWTest::JigHWTest(){

	display = new Display(160, 128);
	canvas = display->getBaseSprite();
	display->getTft()->setPanel(panel());
	display->begin();
	display->getTft()->setRotation(1);
	display->swapBytes(false);

	canvas = display->getBaseSprite();
	canvas->clear(TFT_BLACK);
	display->commit();
}

void JigHWTest::start(){
	Serial.println();
	Serial.printf("TEST:begin:%llx\n", ESP.getEfuseMac());

	Serial.println("TEST:passall");
	canvas->clear(TFT_BLACK);
	canvas->setTextColor(TFT_GOLD);
	canvas->setTextWrap(false, false);
	canvas->setTextDatum(textdatum_t::middle_center);

	canvas->setTextFont(0);
	canvas->setTextSize(1);
	canvas->setCursor(0, 6);

	canvas->printCenter("Armstrong test");
	canvas->setCursor(canvas->width() / 2, 16);
	canvas->println();
	display->commit();

	AudioVisualTest();

}

void JigHWTest::AudioVisualTest(){
	ledcSetup(0, 1000, 8);
	ledcWrite(0, 255);
	ledcAttachPin(LED_G, 0);

	while(true){
		float val = (127.5 * sin(((2 * PI) / (1000.0)) * millis())) + 127.5;
		Serial.println(val);
		ledcWrite(0, val);

		if(millis() - flashTimer >= FlashTime){
			flashTimer = millis();
			show = !show;

			if(show){
				canvas->clear(TFT_BLACK);
				canvas->setTextColor(TFT_GOLD);
				canvas->setTextWrap(false, false);
				canvas->setTextDatum(textdatum_t::middle_center);

				canvas->setTextFont(0);
				canvas->setTextSize(1);
				canvas->setCursor(0, 6);

				canvas->printCenter("Armstrong test");
				canvas->setCursor(canvas->width() / 2, 16);
				canvas->println();

				canvas->setCursor(30, canvas->height() / 2);
				canvas->setTextSize(2);
				canvas->setTextColor(TFT_GREEN);
				canvas->print("All OK!");

				display->commit();
			}else{
				canvas->clear(TFT_BLACK);
				canvas->setTextColor(TFT_GOLD);
				canvas->setTextWrap(false, false);
				canvas->setTextDatum(textdatum_t::middle_center);

				canvas->setTextFont(0);
				canvas->setTextSize(1);
				canvas->setCursor(0, 6);

				canvas->printCenter("Armstrong test");
				canvas->setCursor(canvas->width() / 2, 16);
				canvas->println();
				display->commit();
			}
		}
	}
}
