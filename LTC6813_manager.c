#include "LTC6813_manager.h"

void LTC6813_manager_init(LTC6813_manager_t *p_inst, LTC6813_manager_cfg_t *p_cfg)
{
    if((p_inst != NULL) && (p_cfg != NULL))
    {
        p_inst->cfg = p_cfg;

        p_inst->md = 2;
        p_inst->dcp = 0;
        p_inst->ch = 0;
        p_inst->pup = 0;
        p_inst->st = 1;
        p_inst->chg = 0;
        p_inst->chst = 0;

        p_inst->cfgar[6] = 0;
        p_inst->cfgbr[6] = 0;
        p_inst->cvar[6] = 0;
        p_inst->cvbr[6] = 0;
        p_inst->cvcr[6] = 0;
        p_inst->cvdr[6] = 0;
        p_inst->cver[6] = 0;
        p_inst->cvfr[6] = 0;
        p_inst->avar[6] = 0;
        p_inst->avbr[6] = 0;
        p_inst->avcr[6] = 0;
        p_inst->avdr[6] = 0;
        p_inst->star[6] = 0;
        p_inst->stbr[6] = 0;
        p_inst->comm[6] = 0;
        p_inst->sctrl[6] = 0;
        p_inst->pwmr[6] = 0;
        p_inst->psr[6] = 0;

        p_inst->state = LTC6813_STATE_IDLE;
        p_inst->cmd = LTC6813_CMD_NONE;
    }
    else
    {
        // error handler
    }
}

void LTC6813_manager_task(LTC6813_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
         if(!p_inst->busy_flag)
        {
            switch(p_inst->cmd)
            {
                case LTC6813_CMD_NONE:

                break;

                case LTC6813_CMD_WRITE_CFG:

                break;

                case LTC6813_CMD_POLL_CELLS:

                break;

                case LTC6813_CMD_POLL_GPIOS:

                break;

                case LTC6813_CMD_BALANCE:

                break;
            }
        }
    }
    else
    {
        // error handler
    } 
}

bool LTC6813_manager_cmd_request(LTC6813_manager_t *p_inst, LTC6813_cmd_t cmd)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        p_inst->busy_flag = 0;

        if(p_inst->state == LTC6813_STATE_IDLE)
        {
            p_inst->busy_flag = 1;
        }

    return p_inst->busy_flag;
    }
    else
    {
        // error handler
    }
    
}

void LTC6813_manager_poll_cells(LTC6813_manager_t *p_inst)
{
    switch(p_inst->state)
    {
        
    }
}