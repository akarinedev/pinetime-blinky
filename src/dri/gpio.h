#pragma once

#include <stdint.h>
#include <stdbool.h>

void dri_gpio_dir_set(uint8_t pin, bool dir);
void dri_gpio_output_set(uint8_t pin, bool value);
void dri_gpio_output_toggle(uint8_t pin);
