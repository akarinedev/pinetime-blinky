/**
 * @file
 * Contains functions to use ARM Semihosting functionality
 * Semihosting documentation in <a href="https://developer.arm.com/documentation/dui0471/m/what-is-semihosting-?lang=en">ARM Compiler Software Development Guide, Section 7</a>.
 * @author Akari Neukirch <akari@akarine.dev>
 */

#include "smh.h"

#include <stdint.h>

/**
 * Execute a semihosting operation by number and parameter.
 * @param operation: the operation to perform, referenced from the ARM Compiler Software Development Guide, Section 7
 * @param param: meaning depends on operation
 */
static uint32_t semihosting_exec(uint32_t operation, uint32_t parameter) {
	uint32_t return_val;

	asm volatile(
		"MOV r0, %[op];"
		"MOV r1, %[param];"
		"BKPT 0xAB;"
		"MOV %[ret], r0;"
		: [ret] "=r" (return_val)
		: [op] "r" (operation), [param] "r" (parameter)
		: "r0", "r1"
	);

	return return_val;
}

/**
 * Writes one character to the attached debugger.
 * TODO: Currently nonfunctional, -O breaks parameters
 * @param ch: the character to print
 */
void dri_smh_send_char(char ch) {
	semihosting_exec(0x03, (uint32_t) &ch);
}

/**
 * Writes a null-terminated string to the attached debugger.
 * @param string: start address of the string
 */
void dri_smh_send_string(char* string) {
	semihosting_exec(0x04, (uint32_t) string);
}

char dri_smh_read_char() {
	return (char) semihosting_exec(0x07, 0);
}

uint32_t dri_smh_read_line(char* buffer, uint32_t maxlen) {
	uint32_t numchars = 0;
	char c;
	while((c = dri_smh_read_char()) != '\n' && numchars < maxlen) {
		buffer[numchars] = c;
		numchars++;
	}
	buffer[numchars] = '\0'; // Null-terminate
	return numchars;
}
