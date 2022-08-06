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
static uint32_t semihosting_exec(uint32_t operation, uint32_t param) {
	register uint32_t operation_reg asm ("r0") = operation;
	register uint32_t param_reg asm ("r1") = param;

	asm volatile(
		"BKPT 0xAB"
		: "+r" (operation_reg)
		: "r" (param_reg)
	);

	return operation_reg;
}

/**
 * Writes one character to the attached debugger.
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
