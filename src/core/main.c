/**
 * @file
 * @author Akari Neukirch <akari@akarine.dev>
 * @section Description
 * Main operation of the program. Ties it all together
 */

#include <stdint.h>
#include <stdbool.h>

#include "dri/spi.h"
#include "dri/smh.h"
#include "dri/lcd.h"


/**
 * Sleeps for some time vaguely proportional to input value.
 * Implemented by an empty loop for now.
 * Awful, bad, horrible, inaccurate in every way.
 * TODO: Replace with RTC-based sleep, and eventually interrupt-based
 * @param loops: the number of rounds through the for loop to do
 */
#pragma GCC push_options
#pragma GCC optimize("O0")
static void sleep(long loops) {
	for(int i = 0; i < loops; i++) {}
}
#pragma GCC pop_options

static void init() {
	dri_spi_init();
	dri_lcd_init();
}

/**
 * Main function, does everything and anything!
 */
static void main() {
	dri_smh_send_string("Initializing...\n");

	init();

	dri_smh_send_string("Initialized\n");

	char buf[64];

	/* Toggle LEDs. */
	while (true)
	{
		dri_lcd_backlight_set(3);
		sleep(3000000);
		dri_lcd_backlight_set(0);
		sleep(3000000);
	}
}

/**
 * Entry point from assembly-land into c-land
 */
int _start(void)
{
	// Sleep so we don't enter main before gdb attaches
	sleep(3000000);
	main();
	return 0; //Never reached
}



