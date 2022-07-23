#include "itm.h"

#include <stdint.h>
#include <stdbool.h>

uint32_t* const DEMCR_BASE = (uint32_t*) 0xE000EDFC;
uint32_t* const ITM_BASE = (uint32_t*) 0xE0000000;
uint32_t* const ITM_TER_BASE = (uint32_t*) 0xE0000E00;

#define STIM0		0
#define TER0		(0xe00 / 4)

static void init_port(uint8_t port) {
	// Enable port
	// Find associated TERx address
	//uint32_t *terx = ITM_BASE + TER0 + (port / 32);
	//uint32_t *terx = ITM_BASE + TER0;
	// Set associated bit to one
	//*terx |= (1 << port % 32);

	ITM_TER_BASE[0] |= (1 << 0);
}


static void send_word(uint8_t port, uint32_t data) {
	// Find associated STIMx address
	//uint32_t *stimx = ITM_BASE + STIM0 + port;
	//uint32_t *stimx = ITM_BASE;
	// Send data
	//*stimx = data;

	ITM_BASE[port] = data;
}

void dri_itm_init() {
	DEMCR_BASE[0] |= (1 << 24);

	init_port(0);

}

void dri_itm_test() {
	uint32_t data = (uint32_t) "1234567\0";
	send_word(0, data);
}
