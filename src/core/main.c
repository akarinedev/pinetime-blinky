#include <stdbool.h>
#include <stdint.h>
#include "lib/nrf/boards.h"

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
	nrf_gpio_cfg_output(LCD_BL_PIN);

	/* Toggle LEDs. */
	while (true)
	{
		NRF_GPIO->OUT ^= LCD_BL_BIT;
		sleep(3000000);
	}
}



