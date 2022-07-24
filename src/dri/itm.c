/**
 * @file
 * WIP implementation of a driver for the Arm ITM trace system, used as a printf system.
 * Information sourced from ARMv7-M Architecture Reference Manual.
 * @author Akari Neukirch <akari@akarine.dev>
 */


#include "itm.h"

#include <stdint.h>
#include <stdbool.h>

/// Base address of DEMCR register bank
uint32_t* const DEMCR_BASE = (uint32_t*) 0xE000EDFC;
/// Base address of ITM register bank
uint32_t* const ITM_BASE = (uint32_t*) 0xE0000000;
/// Start address for trace enable section of ITM register bank
uint32_t* const ITM_TER_BASE = (uint32_t*) 0xE0000E00;


/**
 * Initialize ITM port by enabling the associated Trace Enable Register.
 * @param port: the port to initialize
 */
static void init_port(uint8_t port) {
	uint32_t* address = ITM_TER_BASE + port / 32;
	uint32_t mask = (1 << (port % 32));
	*address |= mask;
}

/**
 * Send word via ITM trace.
 * @param port: ITM port to send over
 * @param data: the word to send
 */
static void send_word(uint8_t port, uint32_t data) {
	ITM_BASE[port] = data;
}

/**
 * Initialize ITM subsystem.
 */
void dri_itm_init() {
	// Enable SWO trace system
	DEMCR_BASE[0] |= (1 << 24);

	// Initialize port 0, just for testing here
	init_port(0);

}

/**
 * Test ITM port 0 by sending one word of data.
 */
void dri_itm_test() {
	uint32_t data = (uint32_t) "1234567\0";
	send_word(0, data);
}
