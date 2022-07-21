#include <stdint.h>
#include <stdbool.h>

#include "dri/gpio.h"

// LED definitions
#define LCD_BL_PIN      23
#define LCD_BL_BIT      (1 << LCD_BL_PIN)

#pragma GCC push_options
#pragma GCC optimize("O0")
void sleep(long loops) {
//	asm("hlt");

	for(int i = 0; i < loops; i++) {}
}
#pragma GCC pop_options

/**
 * @brief Function for application main entry.
 */
int _start(void)
{
	dri_gpio_set_dir_output(LCD_BL_PIN);
	dri_gpio_set_output(LCD_BL_PIN, true);

	/* Toggle LEDs. */
	while (true)
	{
		dri_gpio_toggle_output(LCD_BL_PIN);
		sleep(3000000);
	}
}



