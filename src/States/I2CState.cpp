#include "I2CState.h"
#include <Wire.h>
#include <driver/i2c.h>
#include <Armstrong.h>
#include <Loop/LoopManager.h>

static const char* tag = "I2CState";

void I2CState::onStart(){
	i2c_config_t cfg = {
			.mode = I2C_MODE_SLAVE,
			.sda_io_num = (gpio_num_t) 14,
			.sda_pullup_en = GPIO_PULLUP_ENABLE,
			.scl_io_num = (gpio_num_t) 27,          // select GPIO specific to your project
			.scl_pullup_en = GPIO_PULLUP_ENABLE
	};

	cfg.slave = {
			.addr_10bit_en = false,
			.slave_addr = 0x23
	};

	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_1, &cfg));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_1, I2C_MODE_SLAVE, 256, 256, 0));
	LoopManager::addListener(this);
}

void I2CState::onStop(){
	LoopManager::removeListener(this);
	i2c_driver_delete(I2C_NUM_1);
}

void I2CState::loop(uint micros){
	uint8_t data[2];
	int count = i2c_slave_read_buffer(I2C_NUM_1, data, 2, portMAX_DELAY);
	if(count != 2){
		ESP_LOGW(tag, "I2C Read error, received %d bytes, expected 1 or 2 bytes\n", count);
		return;
	}

	if(data[0] == ReadByte){
		uint8_t motor = data[1];
		uint8_t pos = 0;

		if(motor < 4){
			pos = Motors.getPos((Motor) motor);
		}

		int ret = i2c_slave_write_buffer(I2C_NUM_1, &pos, 1, portMAX_DELAY);
		if(ret != 1){
			ESP_LOGW(tag, "I2C Write error, couldn't send Servo position\n");
		}
	}else{
		uint8_t motor = data[0];
		uint8_t pos = data[1];

		if(motor < 4){
			Motors.setPos((Motor) motor, pos);
		}
	}
}
