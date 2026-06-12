/**
 * @file io_pwm.c
 * @author notwe
 * @date 2026-05-03
 * @brief pwm io abstraction source
 */

#include <stddef.h>

#include "io_pwm.h"

pwm_status_t IO_PWM_Init(io_pwm_t *p_inst, const io_pwm_cfg_t *p_cfg)
{
    pwm_status_t status = PWM_STATUS_ERROR_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL))
    {
        p_inst->cfg = p_cfg;

        p_inst->last_duty_value = 0U;

        if((p_inst->cfg->Load_Duty_Cycle != NULL))
        {
            p_inst->is_init = true;

            status = PWM_STATUS_OK;
        }
        else
        {
            status = PWM_STATUS_ERROR_NULL_POINTER;
        }
    }
    else
    {
        status = PWM_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

pwm_status_t IO_PWM_Set_Duty_Cycle(io_pwm_t *p_inst, uint16_t duty_value)
{
    pwm_status_t status = PWM_STATUS_OK;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            p_inst->cfg->Load_Duty_Cycle(duty_value);

            p_inst->last_duty_value = duty_value;
        }
        else 
        {
            status = PWM_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = PWM_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

pwm_status_t IO_PWM_Get_Duty_Cycle(io_pwm_t *p_inst, uint16_t *p_out)
{
    pwm_status_t status = PWM_STATUS_OK;

    uint16_t duty_value = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            duty_value = p_inst->last_duty_value;
        }
        else 
        {
            status = PWM_STATUS_ERROR_NOT_INIT;
        }

        *p_out = duty_value;
    }
    else 
    {
        status = PWM_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}
