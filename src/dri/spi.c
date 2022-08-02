/**
 * @file
 * WIP SPI driver
 * @author Akari Neukirch <akari@akarine.dev>
 */


#include "spi.h"

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

/// Base address for the SPIM register bank
uint32_t* const SPI_BASE = (uint32_t*) 0x40003000;

/// Task to start SPI transfer
#define TASKS_START		(0x010 / 4)
/// Event to signify event completion
#define EVENTS_STOPPED	(0x104 / 4)
/// Enable SPIM subsystem
#define ENABLE			(0x500 / 4)
/// Pin select for SPI clock
#define PSEL_SCK		(0x508 / 4)
/// Pin select for Master-out, slave-in
#define PSEL_MOSI		(0x50C / 4)
/// Pin select for Master-in, slave-out
#define PSEL_MISO		(0x510 / 4)
/// SPI frequency
#define FREQUENCY		(0x524 / 4)
/// Receieve data buffer pointer
#define RXD_PTR			(0x534 / 4)
/// Receive data buffer length
#define RXD_MAXCNT		(0x538 / 4)
/// Transmit data buffer pointer
#define TXD_PTR			(0x544 / 4)
/// Transmit data buffer length
#define TXD_MAXCNT		(0x548 / 4)

/**
 * Initialize SPI subsystem.
 */
void dri_spi_init() {
	// Setup gpio pins as input/output
	dri_gpio_dir_set(2, true);
	dri_gpio_dir_set(3, true);
	dri_gpio_dir_set(4, false);

	// Tell SPI which pins to use
	SPI_BASE[PSEL_SCK] = 2;
	SPI_BASE[PSEL_MOSI] = 3;
	SPI_BASE[PSEL_MISO] = 4;

	// Set SPI frequency to 8Mbps, the maximum supported
	SPI_BASE[FREQUENCY] = 0x80000000;

	// Enable SPIM
	SPI_BASE[ENABLE] = 7;
}

/**
 * Transmit data over SPI.
 * @param buffer: Buffer to transmit data from
 * @param buf_len: Buffer length
 */
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

/**
 * Receive data over SPI.
 * @param buffer: Buffer to receive data to
 * @param buf_len: Buffer length
 */
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
