/**
 * @file
 * Contains wrapper calls to manage GPIO pins.
 * @author Akari Neukirch <akari@akarine.dev>
 */

#include "gpio.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint8_t unused1[0x504];
	uint32_t OUT;
	uint32_t OUTSET;
	uint32_t OUTCLR;
	uint32_t IN;
	uint32_t DIR;
	uint32_t DIRSET;
	uint32_t DIRCLR;
	uint32_t LATCH;
	uint32_t DETECTMODE;
	uint8_t unused2[0x1d8];
	uint32_t PIN_CNF[32];
} gpio_t;

gpio_t* const GPIO = (gpio_t*) 0x50000000;

/**
 * Sets a GPIO pin as an input or output.
 * @param pin: the pin to set direction
 * @param dir: false for input, true for output
 */
void dri_gpio_dir_set(uint8_t pin, bool dir) {
	GPIO->DIRCLR = (1 << pin);
	GPIO->DIRSET = (dir << pin);
}

/**
 * Sets a pin's output high or low.
 * @param pin: the pin to set
 * @param value: true for high, false for low
 */
void dri_gpio_output_set(uint8_t pin, bool value) {
	GPIO->OUTCLR = (1 << pin);
	GPIO->OUTSET = (value << pin);
}

/**
 * Toggles a pin's output.
 * @param pin: the pin to set
 */
void dri_gpio_output_toggle(uint8_t pin) {
	uint32_t mask = 1 << pin;
	GPIO->OUT ^= mask;
}
