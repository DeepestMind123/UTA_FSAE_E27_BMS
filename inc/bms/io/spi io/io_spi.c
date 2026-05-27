#include "io_spi.h"

void IO_SPI_Init(io_spi_cfg_t *p_cfg)
{
    if(p_cfg != NULL)
    {
        if((p_cfg->spi_cs_high != NULL) &&
            (p_cfg->spi_cs_low != NULL) &&
            (p_cfg->spi_byte_exchange))
        {
            
        }
        else 
        {
            
        }
    }
    else 
    {
        
    }
}