/**
 * @file io_i2c.c
 * @author notwe
 * @date 2026-06-05
 * @brief i2c io abstraction source file
 */

#include "io_i2c.h"

i2c_status_t IO_I2C_Init(io_i2c_t *p_inst, io_i2c_cfg_t *p_cfg, uint8_t addr)
{
    i2c_status_t status = I2C_STATUS_ERROR_NOT_INIT;

    bool hal_status;

    if((p_inst != NULL) && (p_cfg != NULL))
    {
        if((p_cfg->init != NULL)
        && (p_cfg->set_clock != NULL)
        && (p_cfg->transmit != NULL)
        && (p_cfg->receive != NULL))
        {
            p_inst->p_cfg = p_cfg;

            hal_status = p_inst->p_cfg->init(addr);

            if(hal_status)
            {
                p_inst->is_init = true;

                status = I2C_STATUS_OK;
            }
        }
        else 
        {
            status = I2C_STATUS_ERROR_NULL_POINTER;
        }
    }
    else
    {
        status = I2C_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

i2c_status_t IO_I2C_Set_Clock(io_i2c_t *p_inst, uint32_t speed)
{
    i2c_status_t status = I2C_STATUS_OK;

    bool hal_status;

    if((p_inst != NULL) && (p_inst->p_cfg))
    {
        if(p_inst->is_init)
        {
            hal_status = p_inst->p_cfg->set_clock(speed);

            if(!hal_status)
            {
                status = I2C_STATUS_ERROR_NOT_INIT;
            }
        }
        else 
        {
            status = I2C_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = I2C_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

i2c_status_t IO_I2C_Transmit(io_i2c_t *p_inst, const i2c_msg_t *p_msg)
{
    i2c_status_t status = I2C_STATUS_OK;

    bool hal_status;

    if((p_inst != NULL) && (p_inst->p_cfg != NULL) && (p_msg != NULL))
    {
        if(p_inst->is_init)
        {
            hal_status = p_inst->p_cfg->transmit(p_msg);

            if(!hal_status)
            {
                status = I2C_STATUS_ERROR_TRANSMIT_FAIL;
            }
        }
        else 
        {
            status = I2C_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = I2C_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

i2c_status_t IO_I2C_Receive(io_i2c_t *p_inst, i2c_msg_t *p_msg)
{
    i2c_status_t status = I2C_STATUS_OK;

    bool hal_status;

    if((p_inst != NULL) && (p_inst->p_cfg != NULL) && (p_msg != NULL))
    {
        if(p_inst->is_init == true)
        {
            hal_status = p_inst->p_cfg->receive(p_msg);

            if(!hal_status)
            {
                status = I2C_STATUS_ERROR_RECEIVE_FAIL;
            }
        }
        else 
        {
            status = I2C_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = I2C_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}
