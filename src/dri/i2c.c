/**
 * @file
 * WIP I2C Master driver
 * Documentation: <a href="https://infocenter.nordicsemi.com/pdf/nRF52832_PS_v1.8.pdf">nRF52832 Product Specification</a>, Section 33.
 * @author Akari Neukirch <akari@akarine.dev>
 */


#include "i2c.h"

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"


typedef struct {
	struct {
		uint32_t STARTRX;
		uint8_t unused1[0x004];
		uint32_t STARTTX;
		uint8_t unused2[0x008];
		uint32_t STOP;
		uint8_t unused3[0x004];
		uint32_t SUSPEND;
		uint32_t RESUME;
		uint8_t unused4[0x0dc];
	} TASKS;
	struct {
		uint8_t unused1[0x004];
		uint32_t STOPPED;
		uint8_t unused2[0x01c];
		uint32_t ERROR;
		uint8_t unused3[0x020];
		uint32_t SUSPENDED;
		uint32_t RXSTARTED;
		uint32_t TXSTARTED;
		uint8_t unused4[0x008];
		uint32_t LASTRX;
		uint32_t LASTTX;
		uint8_t unused5[0x09c];
	} EVENTS;
	uint32_t SHORTS;
	uint8_t unused1[0x0fc];
	uint32_t INTEN;
	uint32_t INTENSET;
	uint32_t INTENCLR;
	uint8_t unused2[0x1b8];
	uint32_t ERRORSRC;
	uint8_t unused3[0x038];
	uint32_t ENABLE;
	uint8_t unused4[0x004];
	uint32_t PSEL_SCL;
	uint32_t PSEL_SDA;
	uint8_t unused5[0x014];
	uint32_t FREQUENCY;
	uint8_t unused6[0x00c];
	uint32_t RXD_PTR;
	uint32_t RXD_MAXCNT;
	uint32_t RXD_AMOUNT;
	uint32_t RXD_LIST;
	uint32_t TXD_PTR;
	uint32_t TXD_MAXCNT;
	uint32_t TXD_AMOUNT;
	uint32_t TXD_LIST;
	uint8_t unused7[0x034];
	uint32_t ADDRESS;
} twim_t;

volatile twim_t* const TWIM0 = (twim_t*) 0x40003000;
volatile twim_t* const TWIM1 = (twim_t*) 0x40004000;

#define TWIM TWIM0

#define PIN_SCL 7
#define PIN_SDA 6


/**
 * Initialize I2C subsystem.
 */
void dri_i2c_init() {
	// Tell TWIM which pins to use
	TWIM->PSEL_SCL = PIN_SCL;
	TWIM->PSEL_SDA = PIN_SDA;

	// Set I2C frequency to 250kbps, maximum 400kbps
	TWIM->FREQUENCY = 0x04000000;

	// Enable STOP shortcuts
	TWIM->SHORTS = 0x00001200;

	// Enable STOPPED Event
	TWIM->INTENSET = 0x00000002;

	// Enable TWIM
	TWIM->ENABLE = 6;
}

/**
 * Simple data transmit over I2C, not for register-based devices.
 * @param address: i2c address to write data to
 * @param buffer: Buffer to transmit data from
 * @param buf_len: Buffer length
 */
static void simple_tx(uint8_t address, uint8_t* buffer, uint8_t buf_len) {
	TWIM->ADDRESS = (uint32_t) address;
	TWIM->TXD_PTR = (uint32_t) buffer;
	TWIM->TXD_MAXCNT = (uint32_t) buf_len;

	TWIM->TASKS.STARTTX = 1;

	// Wait for transmission end
	// TODO: asynchronous waiting, interrupts or something
	while(TWIM->EVENTS.STOPPED == 0) {}
	// Reset stop flag
	TWIM->EVENTS.STOPPED = 0;
}

/**
 * Simple data receive over I2C, not for register-based devices.
 * @param address: i2c address to receive data from
 * @param buffer: Buffer to receive data to
 * @param buf_len: Buffer length
 */
static void simple_rx(uint8_t address, uint8_t* buffer, uint8_t buf_len) {
	TWIM->ADDRESS = (uint32_t) address;
	TWIM->RXD_PTR = (uint32_t) buffer;
	TWIM->RXD_MAXCNT = (uint32_t) buf_len;

	TWIM->TASKS.STARTRX = 1;

	// Wait for transmission end
	// TODO: asynchronous waiting, interrupts or something
	while(TWIM->EVENTS.STOPPED == 0) {}
	// Reset stop flag
	TWIM->EVENTS.STOPPED = 0;
}

/**
 * Write a register to an I2C peripheral
 * @param address: i2c device to write data to
 * @param reg: Register to write to
 * @param data: Data to write to register
 */
void dri_i2c_register_write(uint8_t address, uint8_t reg, uint8_t data) {
	uint8_t buf[2] = {reg, data};
	simple_tx(address, buf, 2);
}

/**
 * Read a register from an I2C peripheral
 * @param address: i2c device to receive data from
 * @param reg: Register to read from
 * @return: The read byte
 */
uint8_t dri_i2c_register_read(uint8_t address, uint8_t reg) {
	uint8_t output = 0;
	simple_tx(address, &reg, 1);
	simple_rx(address, &output, 1);
	return output;
}
