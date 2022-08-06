#pragma once

#include <stdint.h>

void dri_smh_send_char(char ch);
void dri_smh_send_string(char* string);
char dri_smh_read_char();
uint32_t dri_smh_read_line(char* buffer, uint32_t maxlen);
