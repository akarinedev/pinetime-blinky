#pragma once

#include <stdint.h>
#include <stdbool.h>

void dri_gpio_set_dir_input(uint8_t pin);
void dri_gpio_set_dir_output(uint8_t pin);
void dri_gpio_set_output(uint8_t pin, bool value);
void dri_gpio_toggle_output(uint8_t pin);
