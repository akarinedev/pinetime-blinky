#pragma once

#include <stdint.h>

void dri_i2c_init();
void dri_i2c_register_write(uint8_t address, uint8_t reg, uint8_t data);
uint8_t dri_i2c_register_read(uint8_t address, uint8_t reg);
