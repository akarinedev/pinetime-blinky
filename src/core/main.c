/**
 * You know what a main.c does
 * Glues it all together
 */

#include <stdint.h>
#include <stdbool.h>

//#include "dri/gpio.h"
//#include "dri/spi.h"
#include "dri/lcd.h"

#define LCD_BL_HIGH_PIN      23

/**
 * Sleeps a specificed number of iterations of a loop
 * Awful, bad, horrible, inaccurate in every way
 * To be replaced soon
 * @param loops: the number of rounds through the for loop to do
 */
#pragma GCC push_options
#pragma GCC optimize("O0")
void sleep(long loops) {
//	asm("hlt");

	for(int i = 0; i < loops; i++) {}
}
#pragma GCC pop_options

/**
 * Entry point from assembly-land into c-land
 */
int _start(void)
{
	//dri_gpio_set_dir(LCD_BL_HIGH_PIN, true);
	//dri_gpio_set_output(LCD_BL_HIGH_PIN, true);

	dri_lcd_init();

	/* Toggle LEDs. */
	while (true)
	{
		dri_lcd_backlight_set(3);
		//dri_gpio_toggle_output(LCD_BL_HIGH_PIN);
		sleep(3000000);
		dri_lcd_backlight_set(0);
		sleep(3000000);
	}
}



