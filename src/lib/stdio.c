/**
 * @file
 * Partial C standard library stdio implementation
 * Documentation: C99 Standard, Section 7.19.
 * @author Akari Neukirch <akari@akarine.dev>
 */


#include "stdio.h"

#include "dri/smh.h"


/**
 * Print to semihosting output using printf syntax.
 * @param format: formatting string
 * @return: Number of characters successfully printed, negative if there was an error
 */
int printf(const char* restrict format, ...) {
	char* head = format;\
	char curchar;
	while((curchar = *head) != '\0') {
		if(curchar == '%') {

		}
		else if(curchar == '\\') {

		}
		else {
			dri_smh_send_char(curchar);
			head++;
		}
	}
	// Placeholder
	return 0;
}
