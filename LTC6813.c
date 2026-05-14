#include "LTC6813.h"

void LTC6813_init(LTC6813_t *p_inst, LTC6813_cfg_t *p_cfg)
{
    if((p_inst != NULL) && (p_cfg != NULL))
    {
        p_inst->cfg = p_cfg;

        // command definitions
        p_inst->cfg->WRCFGA = 0x01;
        p_inst->cfg->WRCFGB = 0x24;
        p_inst->cfg->RDCFGA = 0x02;
        p_inst->cfg->RDCFGB = 0x26;
        p_inst->cfg->RDCVA = 0x04;
        p_inst->cfg->RDCVB = 0x06;
        p_inst->cfg->RDCVC = 0x08;
        p_inst->cfg->RDCVD = 0x0A;
        p_inst->cfg->RDCVE = 0x09;
        p_inst->cfg->RDCVF = 0x0B;
        p_inst->cfg->RDAUXA = 0x0C;
        p_inst->cfg->RDAUXB = 0x0E;
        p_inst->cfg->RDAUXC = 0x0D;
        p_inst->cfg->RDAUXD = 0x0F;
        p_inst->cfg->RDSTATA = 0x10;
        p_inst->cfg->RDSTATB = 0x12;
        p_inst->cfg->WRSCTRL = 0x14;
        p_inst->cfg->WRPWM = 0x20;
        p_inst->cfg->WRPSB = 0x1C;
        p_inst->cfg->RDSCTRL = 0x16;
        p_inst->cfg->RDPWM = 0x22;
        p_inst->cfg->RDPSB = 0x1E;
        p_inst->cfg->STSCTRL = 0x19;
        p_inst->cfg->CLRSCTRL = 0x18;
        p_inst->cfg->MUTE = 0x28;
        p_inst->cfg->UNMUTE = 0x29;

        // initialize variable commands to basic (0s where variables are)
        p_inst->cfg->ADCV = 0x0260;
        p_inst->cfg->ADOW = 0x0214;
        p_inst->cfg->CVST = 0x0207;
        p_inst->cfg->ADOL = 0x0201;
        p_inst->cfg->ADAX = 0x0460;
        p_inst->cfg->ADAXD = 0x0400;
        p_inst->cfg->AXOW = 0x0410;
        p_inst->cfg->AXST = 0x0407;
        p_inst->cfg->ADSTAT = 0x0468;
        p_inst->cfg->ADSTATD = 0x0408;
        p_inst->cfg->STATST = 0x040F;
        p_inst->cfg->ADCVAX = 0x046F;
        p_inst->cfg->ADCVSC = 0x0467; // "6 7" *hand gesture*
        
        // aaand here's the rest of the 16 bit commands
        p_inst->cfg->CLRCELL = 0x0711;
        p_inst->cfg->CLRAUX = 0x0712;
        p_inst->cfg->CLRSTAT = 0x0713;
        p_inst->cfg->PLADC = 0x0714;
        p_inst->cfg->DIAGN = 0x0715;
        p_inst->cfg->WRCOMM = 0x0721;
        p_inst->cfg->RDCOMM = 0x0722;
        p_inst->cfg->STCOMM = 0x0723;

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
        uint16_t comm = (uint16_t)p_inst->cfg->ADCV | 
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
        uint16_t comm = (uint16_t)p_inst->cfg->ADOW |
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
        uint16_t comm = (uint16_t)p_inst->cfg->CVST |
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
        uint16_t comm = (uint16_t)p_inst->cfg->ADOL |
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
        uint16_t comm = (uint16_t)p_inst->cfg->ADAX |
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
        uint16_t comm = (uint16_t)p_inst->cfg->ADAXD |
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
        uint16_t comm = (uint16_t)p_inst->cfg->AXOW |
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
        uint16_t comm = (uint16_t)p_inst->cfg->AXST |
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
        uint16_t comm = (uint16_t)p_inst->cfg->ADSTAT |
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
        uint16_t comm = (uint16_t)p_inst->cfg->ADSTATD |
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
        uint16_t comm = (uint16_t)p_inst->cfg->STATST |
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
        uint16_t comm = (uint16_t)p_inst->cfg->ADCVAX |
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
        uint16_t comm = (uint16_t)p_inst->cfg->ADCVSC |
                        ((uint16_t)p_inst->MD << MD_SHIFT) |
                        ((uint16_t)p_inst->DCP << DCP_SHIFT);

        return comm;
    }
    else
    {
        // error handler
    } return 0;
}