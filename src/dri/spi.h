#pragma once

#include <stdint.h>

void dri_spi_init();
void dri_spi_tx(uint8_t* buffer, uint8_t buf_len);
void dri_spi_rx(uint8_t* buffer, uint8_t buf_len);
