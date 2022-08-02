/**
 * @file
 * WIP implementation of a driver for the Arm ITM trace system, used as a printf system.
 * Information sourced from ARMv7-M Architecture Reference Manual.
 * @author Akari Neukirch <akari@akarine.dev>
 */


#include "itm.h"

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

typedef struct {
	uint32_t TASKS_HFCLKSTART;
	uint32_t TASKS_HFCLKSTOP;
	uint32_t TASKS_LFCLKSTART;
	uint32_t TASKS_LFCLKSTOP;
	uint32_t TASKS_CAL;
	uint32_t TASKS_CTSTART;
	uint32_t TASKS_CTSTOP;
	uint32_t EVENTS_HFCLKSTARTED;
	uint32_t EVENTS_LFCLKSTARTED;
	uint32_t EVENTS_DONE;
	uint32_t EVENTS_CTTO;
	uint8_t unused1[0x1f0];
	uint32_t INTENSET;
	uint32_t INTENCLR;
	uint32_t HFCLKRUN;
	uint32_t HFCLKSTAT;
	uint32_t LFCLKRUN;
	uint32_t LFCLKSTAT;
	uint32_t LFCLKSRCCOPY;
	uint8_t unused2[0xf8];
	uint32_t LFCLKSRC;
	uint32_t CTIV;
	uint32_t TRACECONFIG;
} clock_t;

clock_t* const CLOCK = (clock_t*) 0x40000000;

typedef struct {
	uint32_t STIM[256];
	uint8_t unused1[0xa00];
	uint32_t TER[8];
	uint8_t unused2[0x20];
	uint32_t TPR;
	uint8_t unused3[0x3c];
	uint32_t TCR;
} itm_t;

itm_t* const ITM = (itm_t*) 0xe0000000;

uint32_t* const DEMCR = (uint32_t*) 0xe000edfc;

/**
 * Initialize ITM port by enabling the associated Trace Enable Register.
 * @param port: the port to initialize
 */
static void init_port(uint8_t port) {
	uint8_t address = port / 32;
	uint32_t mask = (1 << (port % 32));
	ITM->TER[address] |= mask;
}

/**
 * Send word via ITM trace.
 * @param port: ITM port to send over
 * @param data: the word to send
 */
static void send_word(uint8_t port, uint32_t data) {
	while((ITM->STIM[port] & 1) == 0) {
		//While not ready, stall
	}

	ITM->STIM[port] = data;
}

/**
 * Initialize ITM subsystem.
 */
void dri_itm_init() {
	dri_gpio_dir_set(18, true);

	// Clock TRACEPORTSPEED = 4MHz, the minimum
	CLOCK->TRACECONFIG |= 3;

	// Clock TRACEMUX, enable SWO
	CLOCK->TRACECONFIG &= ~(3 << 16);
	CLOCK->TRACECONFIG |= (1 << 16);

	// ITM enable
	ITM->TCR |= 1;

	// Enable SWO trace system
	DEMCR[0] |= (1 << 24);

	// Initialize port 0, just for testing here
	init_port(0);
}

/**
 * Test ITM port 0 by sending one word of data.
 */
void dri_itm_test() {
	uint32_t data = (uint32_t) 'a';
	send_word(0, data);
}
