/**
 * @file
 * @author Akari Neukirch <akari@akarine.dev>
 * @section Description
 * Main operation of the program. Ties it all together
 */

#include <stdint.h>
#include <stdbool.h>

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
void sleep(long loops) {
	for(int i = 0; i < loops; i++) {}
}
#pragma GCC pop_options

void semihosting_exec(uint32_t operation, uint32_t param) {
	register uint32_t r2 asm ("r2") = operation;
	register uint32_t r1 asm ("r1") = param;

	asm(
		"MOV r0, r2\n"
		"BKPT 0xAB"
		:
		: "r" (r1),
		  "r" (r2)
		: "r0"
	);
}

void semihosting_test() {
	char c = 'a';
	char nl = '\n';
	semihosting_exec(0x03, (uint32_t) &c);
	semihosting_exec(0x03, (uint32_t) &nl);
}

/**
 * Entry point from assembly-land into c-land
 */
int _start(void)
{
	// Sleep so we don't blow by main before gdb attaches
	sleep(3000000);

	/* Toggle LEDs. */
	while (true)
	{
		dri_lcd_backlight_set(3);
		semihosting_test();
		sleep(3000000);
		dri_lcd_backlight_set(0);
		sleep(3000000);
	}
}



