/**
 * @file bms_config.c
 * @author notwe
 * @date 2026-06-08
 * @brief idc driver source
 */

#include <stddef.h>

#include "ADI.h"
#include "LTC6813.h"
#include "dev_ltc6813.h"
#include "util_time.h"

dev_ltc6813_status_t DEV_LTC6813_Init(dev_ltc6813_t *p_inst, const cell_asic_t *p_cell_asic)
{
    dev_ltc6813_status_t status = LTC6813_STATUS_ERROR_NOT_INIT;

    if((p_inst != NULL) && (p_cell_asic != NULL))
    {
        p_inst->p_cell_asic = p_cell_asic;

        p_inst->md = 2;
        p_inst->dcp = 0;
        p_inst->ch = 0;
        p_inst->pup = 0;
        p_inst->st = 1;
        p_inst->chg = 0;
        p_inst->chst = 0;

        p_inst->cmd_packet[4] = 0;
        p_inst->len = 0;

        p_inst->cmd_buffer = 0;
        p_inst->cv_buffer[18] = 0;
        p_inst->aux_buffer[12] = 0;

        p_inst->busy_flag = 0;
        p_inst->ready_flag = 0;

        p_inst->state = LTC6813_STATE_IDLE;
        p_inst->cmd = LTC6813_CMD_NONE;

        p_inst->is_init = true;

        status = LTC6813_STATUS_OK;
    }
    else
    {
        status = LTC6813_STATUS_ERROR_NULL_POINTER;
    }

    init_PEC15_Table();

    return status;
}

dev_ltc6813_status_t DEV_LTC6813_Task(dev_ltc6813_t *p_inst)
{
    if(p_inst != NULL)
    {
         if(!p_inst->busy_flag)
        {
            switch(p_inst->cmd)
            {
                case LTC6813_CMD_NONE:
                {

                }

                break;
            }
        }
    }
    else
    {
        // error handler
    } 
}

dev_ltc6813_status_t DEV_LTC6813_CMD_Request(dev_ltc6813_t *p_inst, dev_ltc6813_cmd_t cmd, bool *p_out)
{
    dev_ltc6813_status_t status = LTC6813_STATUS_OK;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            if(p_inst->state == LTC6813_STATE_IDLE)
            {
                p_inst->busy_flag = true;
            }
            else 
            {
                p_inst->busy_flag = false;
            }

            *p_out = p_inst->busy_flag;
        }
        else 
        {
            status = LTC6813_STATUS_ERROR_NOT_INIT;
        }
    }
    else
    {
        status = LTC6813_STATUS_ERROR_NULL_POINTER;
    }
    
    return status;
}

dev_ltc6813_status_t DEV_LTC6813_Get_CMD_Packet(dev_ltc6813_t *p_inst, uint16_t val1, uint16_t val2)
{
    if(p_inst != NULL)
    {
        p_inst->cmd_packet[4] = 0U;

        // msb command bit packing
        p_inst->cmd_packet[0] = (uint8_t)(val1 >> 8U);
        p_inst->cmd_packet[1] = (uint8_t)(val1 & 0x00FFU);
        p_inst->cmd_packet[2] = (uint8_t)(val2 >> 8U);
        p_inst->cmd_packet[3] = (uint8_t)(val2 & 0x00FFU);

        p_inst->len = 4U;
    }
}
