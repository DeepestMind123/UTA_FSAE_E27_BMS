#include "LTC6813.h"

uint32_t LTC6813_get_delay_ms(uint8_t md, uint8_t cell_num)
{
    uint16_t cell_delay_ms = 0U;

    if(md < MD_MAX)
    {
        cell_delay_ms = cell_meas_time_us[md] * US_TO_MS;
    }
    else
    {
        cell_delay_ms = cell_meas_time_us[MD_MAX - 1] * US_TO_MS;
    }

    uint32_t total_delay_ms = 0U;

    total_delay_ms = cell_delay_ms * cell_num;

    return total_delay_ms;
}

uint16_t LTC6813_get_ADCV(uint8_t MD, bool DCP, uint8_t CH)
{
    uint16_t command = 0U;

    command = (uint16_t)ADCV | 
                        ((uint16_t)MD << MD_SHIFT) | 
                        ((uint16_t)DCP << DCP_SHIFT) | 
                        ((uint16_t)CH << CH_SHIFT);

    return command;
}

uint16_t LTC6813_get_ADOW(uint8_t MD, bool PUP, bool DCP, uint8_t CH)
{
    uint16_t command = 0U;

    command = (uint16_t)ADOW |
            ((uint16_t)MD << MD_SHIFT) |
            ((uint16_t)PUP << PUP_SHIFT) |
            ((uint16_t)DCP << DCP_SHIFT) |
            ((uint16_t)CH << CH_SHIFT);

    return command;
}

uint16_t LTC6813_get_CVST(uint8_t MD, uint8_t ST)
{
    uint16_t command = 0U;

    command = (uint16_t)CVST |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)ST << ST_SHIFT);
    return command;
}

uint16_t LTC6813_get_ADOL(uint8_t MD, bool DCP)
{
    uint16_t command = 0U;

    command = (uint16_t)ADOL |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)DCP << DCP_SHIFT);

    return command;
}

uint16_t LTC6813_get_ADAX(uint8_t MD, uint8_t CHG)
{
    uint16_t command = 0U;

    command = (uint16_t)ADAX |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)CHG << CHG_SHIFT);

    return command;
}

uint16_t LTC6813_get_ADAXD(uint8_t MD, uint8_t CHG)
{
    uint16_t command = 0U;

    command = (uint16_t)ADAXD |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)CHG << CHG_SHIFT);

    return command;
}

uint16_t LTC6813_get_AXOW(uint8_t MD, bool PUP, uint8_t CHG)
{
    uint16_t command = 0U;

    command = (uint16_t)AXOW |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)PUP << PUP_SHIFT) |
                ((uint16_t)CHG << CHG_SHIFT);

    return command;
}

uint16_t LTC6813_get_AXST(uint8_t MD, uint8_t ST)
{
    uint16_t command = 0U;
    
    command = (uint16_t)AXST |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)ST << ST_SHIFT);

    return command;
}

uint16_t LTC6813_get_ADSTAT(uint8_t MD, uint8_t CHST)
{
    uint16_t command = 0U;

    command = (uint16_t)ADSTAT |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)CHST << CHST_SHIFT);

    return command;
}

uint16_t LTC6813_get_ADSTATD(uint8_t MD, uint8_t CHST)
{
    uint16_t command = 0U;

    command = (uint16_t)ADSTATD |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)CHST << CHST_SHIFT);

    return command;
}

uint16_t LTC6813_get_STATST(uint8_t MD, uint8_t ST)
{
    uint16_t command = 0U;

    command = (uint16_t)STATST |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)ST << ST_SHIFT);

    return command;
}

uint16_t LTC6813_get_ADCVAX(uint8_t MD, bool DCP)
{
    uint16_t command = 0U;

    command = (uint16_t)ADCVAX |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)DCP << DCP_SHIFT);

    return command;
}

uint16_t LTC6813_get_ADCVSC(uint8_t MD, bool DCP)
{
    uint16_t command = 0U;

    command = (uint16_t)ADCVSC |
                ((uint16_t)MD << MD_SHIFT) |
                ((uint16_t)DCP << DCP_SHIFT);

    return command;
}