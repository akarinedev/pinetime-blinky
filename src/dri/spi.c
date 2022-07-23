#include "spi.h"

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

uint32_t* const SPI_BASE = (uint32_t*) 0x40003000;

#define TASKS_START		(0x010 / 4)
#define EVENTS_STOPPED	(0x104 / 4)
#define ENABLE			(0x500 / 4)
#define PSEL_SCK		(0x508 / 4)
#define PSEL_MOSI		(0x50C / 4)
#define PSEL_MISO		(0x510 / 4)
#define FREQUENCY		(0x524 / 4)
#define RXD_PTR			(0x534 / 4)
#define RXD_MAXCNT		(0x538 / 4)
#define TXD_PTR			(0x544 / 4)
#define TXD_MAXCNT		(0x548 / 4)

static bool initialized = false;

void dri_spi_init() {
	// Setup gpio pins as input/output
	dri_gpio_set_dir(2, true);
	dri_gpio_set_dir(3, true);
	dri_gpio_set_dir(4, false);

	// Tell SPI which pins to use
	SPI_BASE[PSEL_SCK] = 2;
	SPI_BASE[PSEL_MOSI] = 3;
	SPI_BASE[PSEL_MISO] = 4;

	// Set SPI frequency to 8Mbps, the maximum supported
	SPI_BASE[FREQUENCY] = 0x80000000;

	// Enable SPIM
	SPI_BASE[ENABLE] = 7;
}

void dri_spi_tx(uint8_t* buffer, uint8_t buf_len) {
	SPI_BASE[TXD_PTR] = (uint32_t) buffer;
	SPI_BASE[TXD_MAXCNT] = buf_len;

	SPI_BASE[TASKS_START] = 1;

	// Wait for transmission end
	// TODO: asynchronous waiting, interrupts or something
	while(SPI_BASE[EVENTS_STOPPED] == 0) {}
	// Reset stopped flag
	SPI_BASE[EVENTS_STOPPED] = 0;
}

void dri_spi_rx(uint8_t* buffer, uint8_t buf_len) {
	SPI_BASE[RXD_PTR] = (uint32_t) buffer;
	SPI_BASE[RXD_MAXCNT] = buf_len;

	SPI_BASE[TASKS_START] = 1;

	// Wait for transmission end
	// TODO: asynchronous waiting, interrupts or something
	while(SPI_BASE[EVENTS_STOPPED] == 0) {}
	// Reset stopped flag
	SPI_BASE[EVENTS_STOPPED] = 0;
}
