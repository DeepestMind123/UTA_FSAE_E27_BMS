/**
 * @file util_pid_ctrl.c
 * @author notwe
 * @date 2026-06-12
 * @brief generic pid control source
 */

#include "util_pid_ctrl.h"

pid_status_t UTIL_PID_Ctrl_Init(pid_ctrl_t *p_inst, const pid_ctrl_cfg_t *p_cfg, const util_time_t *p_time_inst)
{
<<<<<<< Updated upstream
    pid_status_t status = PID_STATUS_ERROR_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL) && (p_time_inst != NULL))
    {
        p_inst->cfg = p_cfg;
        p_inst->time_inst = p_time_inst;

        p_inst->now_val = 0.0f;
        p_inst->last_time_ms = 0U;
        p_inst->is_init = true;

        status = PID_STATUS_OK;
    }
    else
    {
        status = PID_STATUS_ERROR_NULL_POINTER;
=======
    pid_status_t status = PID_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL) && (p_time_inst != NULL))
    {
        if(!p_inst->is_init)
        {
            p_inst->cfg = p_cfg;
            p_inst->time_inst = p_time_inst;

            p_inst->now_val = 0.0f;
            p_inst->last_time_ms = 0U;
            p_inst->is_init = true;

            status = PID_OK;
        }
        else
        {
            status = PID_DBL_INIT;
        }
    }
    else
    {
        status = PID_NULL_PTR;
>>>>>>> Stashed changes
    }

    return status;
}

pid_status_t UTIL_PID_Ctrl_Task(pid_ctrl_t *p_inst, float val, float *p_out)
{
<<<<<<< Updated upstream
    pid_status_t status = PID_STATUS_OK;
=======
    pid_status_t status = PID_OK;
>>>>>>> Stashed changes
    time_status_t time_status;

    float error = 0.0f;
    float integral = 0.0f;
    float deriv = 0.0f;
    float output = 0.0f;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->time_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            p_inst->now_val = val;

            time_status = UTIL_Time_Get_Tick(p_inst->time_inst, &p_inst->now_time_ms);

<<<<<<< Updated upstream
            if(time_status == TIME_STATUS_OK)
=======
            if(time_status == TIME_OK)
>>>>>>> Stashed changes
            {

                uint32_t dt = p_inst->now_time_ms - p_inst->last_time_ms;

                if(p_inst->now_val > p_inst->cfg->setpoint)
                {
                    error = p_inst->now_val - p_inst->cfg->setpoint;
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

                deriv = error / (float)dt;

                output = (error * K_P) + (integral * K_I) + (deriv * K_D);

                if(output > PWM_MAX)
                {
                    output = PWM_MAX;
                }
                else if(output < PWM_MIN)
                {
                    output = PWM_MIN;
                }

                p_inst->last_time_ms = p_inst->now_time_ms;
            }
            else
            {
<<<<<<< Updated upstream
                status = PID_STATUS_ERROR_TIME_ERROR;
=======
                status = PID_TIME_FAULT;
>>>>>>> Stashed changes
            }
        }
        else 
        {
<<<<<<< Updated upstream
            status = PID_STATUS_ERROR_NOT_INIT;
=======
            status = PID_NOT_INIT;
>>>>>>> Stashed changes
        }

        *p_out = output;

    }
    else 
    {
<<<<<<< Updated upstream
        status = PID_STATUS_ERROR_NULL_POINTER;
=======
        status = PID_NULL_PTR;
>>>>>>> Stashed changes
    }

    return status;
}


pid_status_t UTIL_PID_Ctrl_Get_Val(pid_ctrl_t *p_inst, float *p_out) {
<<<<<<< Updated upstream
    pid_status_t status = PID_STATUS_OK;
=======
    pid_status_t status = PID_OK;
>>>>>>> Stashed changes
    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            *p_out = p_inst->now_val;
        }
        else
        {
<<<<<<< Updated upstream
            status = PID_STATUS_ERROR_NOT_INIT;
=======
            status = PID_NOT_INIT;
>>>>>>> Stashed changes
        }
    }
    else
    {
<<<<<<< Updated upstream
        status = PID_STATUS_ERROR_NULL_POINTER;
=======
        status = PID_NULL_PTR;
>>>>>>> Stashed changes
    }
    return status;
}