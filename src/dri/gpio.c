/**
 * @file
 * Contains wrapper calls to manage GPIO pins.
 * @author Akari Neukirch <akari@akarine.dev>
 */

#include "gpio.h"

#include <stdint.h>
#include <stdbool.h>

//TODO: replace with struct
/// the base address of the GPIO register bank
uint32_t* const GPIO_BASE = (uint32_t*) 0x50000000;

/// Write GPIO pin output
#define OUT			(0x504 / 4)
/// Read GPIO pin input
#define IN			(0x510 / 4)
/// Direction of GPIO pin (input/output)
#define DIR			(0x514 / 4)

/**
 * Sets a GPIO pin as an input or output.
 * @param pin: the pin to set direction
 * @param dir: false for input, true for output
 */
void dri_gpio_set_dir(uint8_t pin, bool dir) {
	uint32_t clear_mask = ~(1 << pin);
	GPIO_BASE[DIR] &= clear_mask;
	uint32_t mask = (uint32_t) dir << pin;
	GPIO_BASE[DIR] |= mask;
}

/**
 * Sets a pin's output high or low.
 * @param pin: the pin to set
 * @param value: true for high, false for low
 */
void dri_gpio_set_output(uint8_t pin, bool value) {
	uint32_t clear_mask = ~(1 << pin);
	GPIO_BASE[OUT] &= clear_mask;
	uint32_t mask = (uint32_t) value << pin;
	GPIO_BASE[OUT] |= mask;
}

/**
 * Toggles a pin's output.
 * @param pin: the pin to set
 */
void dri_gpio_toggle_output(uint8_t pin) {
	uint32_t mask = 1 << pin;
	GPIO_BASE[OUT] ^= mask;
}
