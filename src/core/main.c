/**
 * You know what a main.c does
 * Glues it all together
 */

#include <stdint.h>
#include <stdbool.h>

#include "dri/lcd.h"
#include "dri/itm.h"

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
	dri_lcd_init();

	dri_itm_init();

	/* Toggle LEDs. */
	while (true)
	{
		dri_lcd_backlight_set(3);
		dri_itm_test();
		sleep(3000000);
		dri_lcd_backlight_set(0);
		sleep(3000000);
	}
}



