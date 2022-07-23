#include "lcd.h"

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"


#define PIN_RS		18
#define PIN_CS		25
#define PIN_RESET	26
#define PIN_BL_LOW	14
#define PIN_BL_MED	22
#define PIN_BL_HI	23

static int backlight_brightness;

const uint8_t PINS_BL[] = {0, PIN_BL_LOW, PIN_BL_MED, PIN_BL_HI};

void dri_lcd_init() {
	backlight_brightness = 0;

	dri_gpio_set_dir(PIN_RS, true);
	dri_gpio_set_dir(PIN_CS, true);
	dri_gpio_set_dir(PIN_RESET, true);
	dri_gpio_set_dir(PIN_BL_LOW, true);
	dri_gpio_set_dir(PIN_BL_MED, true);
	dri_gpio_set_dir(PIN_BL_HI, true);

	dri_gpio_set_output(PIN_RS, true);
	dri_gpio_set_output(PIN_CS, true);
	dri_gpio_set_output(PIN_RESET, true);
	dri_gpio_set_output(PIN_BL_LOW, true);
	dri_gpio_set_output(PIN_BL_MED, true);
	dri_gpio_set_output(PIN_BL_HI, true);
}


// 0 = off, 1 = low, 2 = medium, 3 = high
void dri_lcd_backlight_set(uint8_t level) {
	if(backlight_brightness != 0) {
		// Turn off current backlight level
		dri_gpio_set_output(PINS_BL[backlight_brightness], true);
	}

	if(level != 0) {
		dri_gpio_set_output(PINS_BL[level], false);
	}
	backlight_brightness = level;
}
