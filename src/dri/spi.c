/**
 * @file
 * WIP SPI driver
 * Documentation: <a href="https://infocenter.nordicsemi.com/pdf/nRF52832_PS_v1.8.pdf">nRF52832 Product Specification</a>, Section 31.
 * @author Akari Neukirch <akari@akarine.dev>
 */


#include "spi.h"

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"


typedef struct {
	uint8_t unused1[0x010];
	uint32_t TASKS_START;
	uint32_t TASKS_STOP;
	uint8_t unused2[0x004];
	uint32_t TASKS_SUSPEND;
	uint32_t TASKS_RESUME;
	uint8_t unused3[0x0e0];
	uint32_t EVENTS_STOPPED;
	uint8_t unused4[0x004];
	uint32_t EVENTS_ENDRX;
	uint8_t unused5[0x004];
	uint32_t EVENTS_END;
	uint8_t unused6[0x004];
	uint32_t EVENTS_ENDTX;
	uint8_t unused7[0x028];
	uint32_t EVENTS_STARTED;
	uint8_t unused8[0x0b0];
	uint32_t SHORTS;
	uint8_t unused9[0x100];
	uint32_t INTENSET;
	uint32_t INTENCLR;
	uint8_t unused10[0x1f4];
	uint32_t ENABLE;
	uint8_t unused11[0x004];
	uint32_t PSEL_SCK;
	uint32_t PSEL_MOSI;
	uint32_t PSEL_MISO;
	uint8_t unused12[0x010];
	uint32_t FREQUENCY;
	uint8_t unused13[0x00c];
	uint32_t RXD_PTR;
	uint32_t RXD_MAXCNT;
	uint32_t RXD_AMOUNT;
	uint32_t RXD_LIST;
	uint32_t TXD_PTR;
	uint32_t TXD_MAXCNT;
	uint32_t TXD_AMOUNT;
	uint32_t TXD_LIST;
	uint32_t CONFIG;
	uint8_t unused14[0x068];
	uint32_t ORC;
} spim_t;

volatile spim_t* const SPIM0 = (spim_t*) 0x40003000;
volatile spim_t* const SPIM1 = (spim_t*) 0x40004000;
volatile spim_t* const SPIM2 = (spim_t*) 0x40023000;

#define SPIM SPIM1

#define PIN_SCK 2
#define PIN_MOSI 3
#define PIN_MISO 4


/**
 * Initialize SPI subsystem.
 */
void dri_spi_init() {
	// Setup gpio pins as input/output
	dri_gpio_dir_set(PIN_SCK, true);
	dri_gpio_dir_set(PIN_MOSI, true);
	dri_gpio_dir_set(PIN_MISO, false);

	// Tell SPI which pins to use
	SPIM->PSEL_SCK = PIN_SCK;
	SPIM->PSEL_MOSI = PIN_MOSI;
	SPIM->PSEL_MISO = PIN_MISO;

	// Set SPI frequency to 8Mbps, the maximum supported
	SPIM->FREQUENCY = 0x80000000;

	// Enable SPIM
	SPIM->ENABLE = 7;
}

/**
 * Transmit data over SPI.
 * @param buffer: Buffer to transmit data from
 * @param buf_len: Buffer length
 */
void dri_spi_tx(uint8_t* buffer, uint8_t buf_len) {
	SPIM->TXD_PTR = (uint32_t) buffer;
	SPIM->TXD_MAXCNT = buf_len;

	SPIM->RXD_MAXCNT = 0;

	SPIM->TASKS_START = 1;

	// Wait for transmission end
	// TODO: asynchronous waiting, interrupts or something
	while(SPIM->EVENTS_STOPPED == 0) {}
	// Reset stop flag
	SPIM->EVENTS_STOPPED = 0;
}

/**
 * Receive data over SPI.
 * @param buffer: Buffer to receive data to
 * @param buf_len: Buffer length
 */
void dri_spi_rx(uint8_t* buffer, uint8_t buf_len) {
	SPIM->RXD_PTR = (uint32_t) buffer;
	SPIM->RXD_MAXCNT = buf_len;

	SPIM->TXD_MAXCNT = 0;

	SPIM->TASKS_START = 1;

	// Wait for transmission end
	// TODO: asynchronous waiting, interrupts or something
	while(SPIM->EVENTS_STOPPED == 0) {}
	// Reset stop flag
	SPIM->EVENTS_STOPPED = 0;
}
