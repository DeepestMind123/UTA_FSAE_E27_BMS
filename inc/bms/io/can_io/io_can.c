/**
 * @file can_io.c
 * @author notwe
 * @date 2026-05-03
 * @brief can io abstraction source file
 */

#include "io_can.h"
#include <stddef.h>

can_status_t IO_CAN_Init(io_can_t *p_inst, const io_can_cfg_t *p_cfg, uint32_t baudrate)
{
    can_status_t status = CAN_STATUS_ERROR_NOT_INIT;

    bool hal_status;

    if((p_inst != NULL) && (p_cfg != NULL))
    {
        p_inst->p_cfg = p_cfg;

        if((p_inst->p_cfg->init != NULL) && (p_inst->p_cfg->receive != NULL) && (p_inst->p_cfg->transmit != NULL))
        {
            hal_status = p_inst->p_cfg->init(baudrate);

            if(hal_status == true)
            {
                p_inst->is_init = true;

                status = CAN_STATUS_OK;
            }
        }
        else 
        {
            status = CAN_STATUS_ERROR_NULL_POINTER;
        }
    }
    else 
    {
        status = CAN_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

can_status_t IO_CAN_Transmit(io_can_t *p_inst, const can_msg_t *p_msg)
{
    can_status_t status = CAN_STATUS_OK;

    bool hal_status;

    if((p_inst != NULL) && (p_inst->p_cfg != NULL) && (p_msg != NULL))
    {
        if(p_inst->is_init)
        {
            hal_status = p_inst->p_cfg->transmit(p_msg);

            if(hal_status != true)
            {
                status = CAN_STATUS_ERROR_TRANSMIT_FAIL;
            }
        }
        else 
        {
            status = CAN_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = CAN_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

can_status_t IO_CAN_Receive(io_can_t *p_inst, can_msg_t *p_msg)
{
    can_status_t status = CAN_STATUS_OK;

    bool hal_status;

    if((p_inst != NULL) && (p_inst->p_cfg != NULL) && (p_msg != NULL))
    {
        if(p_inst->is_init)
        {
            hal_status = p_inst->p_cfg->receive(p_msg);

            if(hal_status != true)
            {
                status = CAN_STATUS_ERROR_RECEIVE_FAIL;
            }
        }
        else 
        {
            status = CAN_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = CAN_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}
