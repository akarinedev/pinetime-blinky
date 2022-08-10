#pragma once

#include <stdint.h>

uint32_t str_copy(const char* src, char* dest, uint32_t max);
uint32_t str_len(const char* s);
void str_reverse_inplace(char* str);
