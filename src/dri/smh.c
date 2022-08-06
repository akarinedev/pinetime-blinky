/**
 * @file
 * Contains functions to use ARM Semihosting functionality
 * Semihosting documentation: <a href="https://developer.arm.com/documentation/dui0471/i/semihosting?lang=en">ARM Compiler toolchain Developing Software</a>
 * @author Akari Neukirch <akari@akarine.dev>
 */

#include "smh.h"

#include <stdint.h>

/**
 * Execute a semihosting operation by number and parameter.
 * @param operation: the operation to perform, referenced from ARM toolchain manual
 * @param param: meaning depends on operation
 */
static void semihosting_exec(uint32_t operation, uint32_t param) {
	register uint32_t r2 asm ("r2") = operation;
	register uint32_t r1 asm ("r1") = param;

	asm(
		"MOV r0, r2\n"
		"BKPT 0xAB"
		:
		: "r" (r1),
		  "r" (r2)
		: "r0"
	);
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
