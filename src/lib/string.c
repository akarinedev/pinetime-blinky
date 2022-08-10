/**
 * @file
 * Partial C standard library string implementation
 * Documentation: C99 Standard, Section 7.21.
 * @author Akari Neukirch <akari@akarine.dev>
 */


#include "string.h"

#include <stdint.h>

uint32_t str_copy(const char* src, char* dest, uint32_t max) {
	for(uint32_t i = 0; i < max; i++) {
		char ch = src[i];
		dest[i] = ch;
		if(ch == '\0') {
			return i;
		}
	}
	dest[max-1] = '\0';
	return max-1;
}

uint32_t str_len(const char* str) {
	uint32_t pos = 0;
	while(str[pos] != 0) {
		pos++;
	}
	return pos;
}

void str_reverse_inplace(char* str) {
	uint32_t length = str_len(str);
	for(uint32_t i = 0; i < length/2; i++) {
		char c = str[i];
		str[i] = str[length-i-1];
		str[length-i-1] = c;
	}
}
