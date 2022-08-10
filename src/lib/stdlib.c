/**
 * @file
 * Partial C standard library stdlib implementation
 * Documentation: C99 Standard, Section 7.20.
 * @author Akari Neukirch <akari@akarine.dev>
 */


#include "stdlib.h"

#include "string.h"


char* itostr(int value, char* str, int base) {
	int runval = value;
	int pos = 0;
	do {
		int digit = runval % base;
		runval /= base;
		char ch;
		if(digit < 10) {
			ch = '0' + digit;
		}
		else {
			ch = 'A' + (digit-10);
		}
		str[pos++] = ch;
	} while(runval != 0);
	if(base == 10 && value < 0) {
		str[pos++] = '-';
	}
	str[pos] = '\0';
	str_reverse_inplace(str);
	return str;
}
