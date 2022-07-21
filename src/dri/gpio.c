#include "gpio.h"

#include <stdint.h>
#include <stdbool.h>

static uint32_t* gpio_base = (uint32_t*) 0x50000000;

void dri_gpio_set_dir_input(uint8_t pin) {
	uint32_t mask = ~(1 << pin);
	gpio_base[325] &= mask;
	//*(gpio_base+0x514) &= mask;
}

void dri_gpio_set_dir_output(uint8_t pin) {
	uint32_t mask = (1 << pin);
	gpio_base[325] |= mask;
	//*(gpio_base+0x514) |= mask;
}

void dri_gpio_set_output(uint8_t pin, bool value) {
	uint32_t clear_mask = ~(1 << pin);
	gpio_base[321] &= clear_mask;
	uint32_t mask = (uint32_t) value << pin;
	gpio_base[321] |= mask;
}

void dri_gpio_toggle_output(uint8_t pin) {
	uint32_t mask = 1 << pin;
	gpio_base[321] ^= mask;
}
