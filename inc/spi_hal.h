#ifndef SPI_HAL_H
#define SPI_HAL_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    void(*spi_cs_high)(uint8_t cs_pin);

    void(*spi_cs_low)(uint8_t cs_pin);

    uint8_t(*spi_byte_exchange)(uint8_t data);

} spi_hal_cfg_t;

spi_hal_cfg_t cfg; 

void spi_hal_init(void);

void spi_transfer_word(uint8_t cs_pin, uint8_t tx, uint8_t *rx);

void spi_transfer_sentence(uint8_t cs_pin, uint8_t *tx, uint8_t *rx, uint8_t len);

#endif