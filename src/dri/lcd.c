/**
 * @file
 * WIP driver for the PineTime LCD screen.
 * Information sourced from: Pine64 wiki, ST7789 datasheet, and Atafruit ST7735 Library.
 * @author Akari Neukirch <akari@akarine.dev>
 */


#include "lcd.h"

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"


/// Command/data pin
#define PIN_RS		18
/// Chip select, active low
#define PIN_CS		25
/// Reset, active low?
#define PIN_RESET	26
/// Backlight low, active low
#define PIN_BL_LOW	14
/// Backlight medium, active low
#define PIN_BL_MED	22
/// Backlight high, active low
#define PIN_BL_HI	23

/// Current backlight brightness value
static int backlight_brightness;

/// Array of backlight brightness pins, for ease of addressing
const uint8_t PINS_BL[] = {0, PIN_BL_LOW, PIN_BL_MED, PIN_BL_HI};

/**
 * Initializes LCD driver.
 * Currently initializes all needed GPIO pins and sets them to disabled.
 */
void dri_lcd_init() {
	backlight_brightness = 0;

	dri_gpio_dir_set(PIN_RS, true);
	dri_gpio_dir_set(PIN_CS, true);
	dri_gpio_dir_set(PIN_RESET, true);
	dri_gpio_dir_set(PIN_BL_LOW, true);
	dri_gpio_dir_set(PIN_BL_MED, true);
	dri_gpio_dir_set(PIN_BL_HI, true);

	dri_gpio_output_set(PIN_RS, false);
	dri_gpio_output_set(PIN_CS, true);
	dri_gpio_output_set(PIN_RESET, true);
	dri_gpio_output_set(PIN_BL_LOW, true);
	dri_gpio_output_set(PIN_BL_MED, true);
	dri_gpio_output_set(PIN_BL_HI, true);
}



/**
 * Set LCD backlight level.
 * @param level: 0 = off, 1 = low, 2 = medium, 3 = high
 */
void dri_lcd_backlight_set(uint8_t level) {
	if(backlight_brightness != 0) {
		// Turn off current backlight level
		dri_gpio_output_set(PINS_BL[backlight_brightness], true);
	}

	if(level != 0) {
		dri_gpio_output_set(PINS_BL[level], false);
	}
	backlight_brightness = level;
}
