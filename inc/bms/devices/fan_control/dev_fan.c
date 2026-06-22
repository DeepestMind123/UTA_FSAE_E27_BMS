/**
 * @file dev_fan.c
 * @author notwe
 * @date 2026-06-12
 * @brief fan driver source
 */

#include "dev_fan.h"

fan_status_t DEV_Fan_Init(dev_fan_t *p_inst, const dev_fan_cfg_t *p_cfg, const io_pwm_t *p_pwm_inst)
{
    fan_status_t status = FAN_STATUS_ERROR_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL) && (p_pwm_inst != NULL))
    {
        p_inst->cfg = p_cfg;
        p_inst->pwm_inst = p_pwm_inst;

        p_inst->now_temp_C = 0U;
        p_inst->duty_value = 0U;
        p_inst->last_time_ms = 0U;

        status = FAN_STATUS_OK;
    }
    else
    {
        status = FAN_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

fan_status_t DEV_Fan_Task(dev_fan_t *p_inst)
{
    fan_status_t status = FAN_STATUS_OK;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->pwm_inst != NULL))
    {
        if(p_inst->is_init)
        {
            p_inst->now_temp_C = 0U; // this needs to be changed to get temp from a getter function, but anyways

            p_inst->now_time_ms = UTIL_Time_Get_Tick();

            float error = 0.0f;
            float integral = 0.0f;
            float output = 0.0f;

            uint32_t dt = p_inst->now_time_ms - p_inst->last_time_ms;

            if(p_inst->now_temp_C > p_inst->cfg->setpoint)
            {
                error = p_inst->now_temp_C - p_inst->cfg->setpoint;
            }

            integral = error * (float)dt;

            if(integral > INTEGRAL_MAX)
            {
                integral = INTEGRAL_MAX;
            }
            else if(integral < INTEGRAL_MIN)
            {
                integral = INTEGRAL_MIN;
            }

            output = (error * K_P) + (integral * K_I);

            if(output > PWM_MAX)
            {
                output = PWM_MAX;
            }
            else if(output < PWM_MIN)
            {
                output = PWM_MIN;
            }

            p_inst->duty_value = (uint16_t)((output / 100.0f) * PWM_MAX_INT);

            p_inst->last_time_ms = p_inst->now_time_ms;
        }
        else 
        {
            status = FAN_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = FAN_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}