#include "gpio.h"

#include <stdint.h>
#include <stdbool.h>

uint32_t* const GPIO_BASE = (uint32_t*) 0x50000000;

void dri_gpio_set_dir_input(uint8_t pin) {
	uint32_t mask = ~(1 << pin);
	GPIO_BASE[325] &= mask;
}

void dri_gpio_set_dir_output(uint8_t pin) {
	uint32_t mask = (1 << pin);
	GPIO_BASE[325] |= mask;
}

void dri_gpio_set_output(uint8_t pin, bool value) {
	uint32_t clear_mask = ~(1 << pin);
	GPIO_BASE[321] &= clear_mask;
	uint32_t mask = (uint32_t) value << pin;
	GPIO_BASE[321] |= mask;
}

void dri_gpio_toggle_output(uint8_t pin) {
	uint32_t mask = 1 << pin;
	GPIO_BASE[321] ^= mask;
}
