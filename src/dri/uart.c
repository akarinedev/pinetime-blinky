 
#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

uint32_t* const UART_BASE = (uint32_t*) 0x40003000;

static bool initialized = false;

void dri_uart_init() {

}

void dri_uart_send(uint32_t* start, uint32_t len) {
	// Prep with data
	UART_BASE[337] = start;
	UART_BASE[338] = len;
	// Start STARTTX task
	UART_BASE[2] = 1;

	// Wait for ENDTX to fire
	while(UART_BASE[72] == 0) {
		volatile a = 0;
	}


}
