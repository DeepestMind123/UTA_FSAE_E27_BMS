#include "io_spi.h"

spi_status_t IO_SPI_Init(io_spi_t *p_inst, io_spi_cfg_t *p_cfg)
{
    spi_status_t status = SPI_STATUS_ERROR_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL))
    {
        p_inst->is_init = false;
        p_inst->cfg = p_cfg;

        if((p_inst->cfg->spi_cs_high != NULL) &&
            (p_inst->cfg->spi_cs_low != NULL) &&
            (p_inst->cfg->spi_byte_exchange != NULL))
        {
            p_inst->is_init = true;

            status = SPI_STATUS_OK;
        }
        else 
        {
            status = SPI_STATUS_ERROR_NULL_POINTER;
        }
    }
    else 
    {
        status = SPI_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

spi_status_t IO_SPI_Transfer_Word(io_spi_t *p_inst, uint8_t cs_pin, uint8_t *tx, uint8_t *rx)
{
    spi_status_t status = SPI_STATUS_OK;

    uint8_t rx_data = 0U;

    uint8_t tx_data = DUMMY_BYTE;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            if(tx != NULL)
            {
                tx_data = *tx;
            }

            p_inst->cfg->spi_cs_low(cs_pin);

            rx_data = p_inst->cfg->spi_byte_exchange(tx_data);

            p_inst->cfg->spi_cs_high(cs_pin);

            if(rx != NULL)
            {
                *rx = rx_data;
            }
        }
        else 
        {
            status = SPI_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = SPI_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

spi_status_t IO_SPI_Transfer_Sentence(io_spi_t *p_inst, uint8_t cs_pin, uint8_t *tx, uint8_t *rx, uint8_t len)
{
    spi_status_t status = SPI_STATUS_OK;

    uint8_t rx_data = 0U;

    uint8_t tx_data = DUMMY_BYTE;

    uint8_t i = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            if(len > 0)
            {
                p_inst->cfg->spi_cs_low(cs_pin);

                for(i = 0U; i < len; i++)
                {
                    if(tx != NULL)
                    {
                        tx_data = tx[i];
                    }

                    rx_data = p_inst->cfg->spi_byte_exchange(tx_data);

                    if(rx != NULL)
                    {
                        rx[i] = rx_data;
                    }
                }

                p_inst->cfg->spi_cs_high(cs_pin);
            }
        }
        else 
        {
            status = SPI_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = SPI_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}