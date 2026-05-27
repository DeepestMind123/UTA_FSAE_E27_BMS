#ifndef IO_SPI_H
#define IO_SPI_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    void(*spi_cs_high)(uint8_t cs_pin);

    void(*spi_cs_low)(uint8_t cs_pin);

    uint8_t(*spi_byte_exchange)(uint8_t data);

} io_spi_cfg_t;

void IO_SPI_Init(io_spi_cfg_t *p_cfg);

void IO_SPI_Transfer_Word(uint8_t cs_pin, uint8_t tx, uint8_t *rx);

void IO_SPI_Transfer_Sentence(uint8_t cs_pin, uint8_t *tx, uint8_t *rx, uint8_t len);

#endif