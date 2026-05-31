/**
 * @file io_pwm.c
 * @author notwe
 * @date 2026-05-03
 * @brief pwm io abstraction source
 */

#include "io_pwm.h"
#include "sys_fault.h"

pwm_status_t IO_PWM_Init(io_pwm_t *p_inst, io_pwm_cfg_t *p_cfg)
{

    pwm_status_t status = PWM_STATUS_ERROR;

    if((p_inst != NULL) && (p_cfg != NULL))
    {
        p_inst->cfg = p_cfg;

        p_inst->last_duty_value = 0U;

        if((p_inst->cfg->Load_Duty_Cycle != NULL))
        {
            status = PWM_STATUS_IDLE;

            p_inst->is_init = true;
        }
        else
        {
            BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
        }
    }
    else
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return status;
}

pwm_status_t IO_PWM_Set_Duty_Cycle(io_pwm_t *p_inst, uint16_t duty_value)
{
    pwm_status_t status = PWM_STATUS_ERROR;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            p_inst->cfg->Load_Duty_Cycle(duty_value);

            p_inst->last_duty_value = duty_value;

            status = PWM_STATUS_IDLE;
        }
        else 
        {
            BMS_Fault_Update(BMS_FAULT_NOT_INIT, true);
        }
    }
    else 
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return status;
}

uint16_t IO_PWM_Get_Duty_Cycle(io_pwm_t *p_inst)
{
    uint16_t duty_value = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        duty_value = p_inst->last_duty_value;
    }
    else 
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return duty_value;
}
