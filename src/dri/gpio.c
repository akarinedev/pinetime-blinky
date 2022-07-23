/*
 * Contains simple wrapper calls to manage GPIO pins
 */

#include "gpio.h"

#include <stdint.h>
#include <stdbool.h>

uint32_t* const GPIO_BASE = (uint32_t*) 0x50000000;

#define OUT			(0x504 / 4)
#define IN			(0x510 / 4)
#define DIR			(0x514 / 4)

/*
 * Sets a GPIO pin as an input/output
 * TODO: Replace with a set input/output function
 * @param pin: the pin to set as input
 * @param dir: false for input, true for output
 */
void dri_gpio_set_dir(uint8_t pin, bool dir) {
	uint32_t clear_mask = ~(1 << pin);
	GPIO_BASE[DIR] &= clear_mask;
	uint32_t mask = (uint32_t) dir << pin;
	GPIO_BASE[DIR] |= mask;
}

/*
 * Sets a pin's output high or low
 * @param pin: the pin to set
 * @param value: true for high, false for low
 */
void dri_gpio_set_output(uint8_t pin, bool value) {
	uint32_t clear_mask = ~(1 << pin);
	GPIO_BASE[OUT] &= clear_mask;
	uint32_t mask = (uint32_t) value << pin;
	GPIO_BASE[OUT] |= mask;
}

/*
 * Toggles a pin's highness
 * @param pin: the pin to set
 */
void dri_gpio_toggle_output(uint8_t pin) {
	uint32_t mask = 1 << pin;
	GPIO_BASE[OUT] ^= mask;
}
