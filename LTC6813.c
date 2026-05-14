#include "LTC6813.h"

void LTC6813_init(LTC6813_t *p_inst, LTC6813_cfg_t *p_cfg)
{
    if((p_inst != NULL) && (p_cfg != NULL))
    {
        p_inst->cfg = p_cfg;

        // set init configs, these can change
        p_inst->MD = 2;     // normal mode
        p_inst->DCP = 0;    // discharging not permitted
        p_inst->CH = 0;     // all cells
        p_inst->PUP = 0;    // pull-down
        p_inst->ST = 1;     // i am not entirely sure what distinction exists between mode 1 and mode 2
        p_inst->CHG = 0;    // all GPIO pins
        p_inst->CHST = 0;    // i have no idea, these are all fault flags that i need to learn

        // init state to ready if keeping state machine
        // tbh there's just a lot going on so i may just do this function by function
    }
}

uint16_t LTC6813_get_ADCV(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)ADCV | 
                        ((uint16_t)p_inst->MD << MD_SHIFT) | 
                        ((uint16_t)p_inst->DCP << DCP_SHIFT) | 
                        ((uint16_t)p_inst->CH << CH_SHIFT);

        return comm;
    }
    else
    {
        // log NULL pointer error (I really need to make some error handlers)
    } return 0;
}

uint16_t LTC6813_get_ADOW(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)ADOW |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->PUP << PUP_SHIFT) |
                        ((uint16_t)p_inst->DCP << DCP_SHIFT) |
                        ((uint16_t)p_inst->CH << CH_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_CVST(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)CVST |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->ST << ST_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_ADOL(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)ADOL |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->DCP << DCP_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_ADAX(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)ADAX |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->CHG << CHG_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_ADAXD(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)ADAXD |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->CHG << CHG_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_AXOW(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)AXOW |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->PUP << PUP_SHIFT) |
                        ((uint16_t)p_inst->CHG << CHG_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_AXST(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)AXST |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->ST << ST_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_ADSTAT(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)ADSTAT |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->CHST << CHST_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_ADSTATD(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)ADSTATD |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->CHST << CHST_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_STATST(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)STATST |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->ST << ST_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_ADCVAX(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)ADCVAX |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->DCP << DCP_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}

uint16_t LTC6813_get_ADCVSC(LTC6813_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        uint16_t comm = (uint16_t)ADCVSC |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->DCP << DCP_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}