#ifndef IO_SPI_H
#define IO_SPI_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

const uint8_t DUMMY_BYTE = 0xFFU;

typedef enum
{
    SPI_STATUS_OK = 0,
    SPI_STATUS_ERROR_NULL_POINTER,
    SPI_STATUS_ERROR_NOT_INIT
} spi_status_t;

typedef const struct
{
    void(*spi_cs_high)(uint8_t cs_pin);
    void(*spi_cs_low)(uint8_t cs_pin);
    uint8_t(*spi_byte_exchange)(uint8_t data);

} io_spi_cfg_t;

typedef struct
{
    bool is_init;
    spi_status_t status;
    io_spi_cfg_t *cfg;

} io_spi_t;

spi_status_t IO_SPI_Init(io_spi_t *p_inst, io_spi_cfg_t *cfg);

spi_status_t IO_SPI_Transfer_Word(io_spi_t *p_inst, uint8_t cs_pin, uint8_t *tx, uint8_t *rx);

spi_status_t IO_SPI_Transfer_Sentence(io_spi_t *p_inst, uint8_t cs_pin, uint8_t *tx, uint8_t *rx, uint8_t len);

#endif