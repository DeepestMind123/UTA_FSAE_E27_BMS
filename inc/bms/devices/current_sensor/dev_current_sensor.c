/**
 * @file DEV_current.c
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor device driver source
 */

#include "dev_current_sensor.h"

current_sensor_status_t DEV_Current_Sensor_Init(dev_current_sensor_t *p_inst, const dev_current_sensor_cfg_t *p_cfg, const io_adc_t *p_adc_inst, const util_time_t *p_time_inst)
{
    current_sensor_status_t status = CURRENT_SENSOR_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL) && (p_adc_inst != NULL) && (p_time_inst))
    {
        // Initialize instanced variables to safe values
        p_inst->cfg = p_cfg;
        p_inst->adc_inst = p_adc_inst;
        p_inst->time_inst = p_time_inst;

        p_inst->cal_acc = 0U;
        p_inst->cal_count = 0U;
        p_inst->raw_offset = 0U;
        p_inst->start_time = 0U;

        p_inst->adc_timeout_ms = p_inst->adc_inst->cfg->adc_timeout;
        p_inst->current_timeout = p_inst->adc_timeout_ms;

        p_inst->state = CURRENT_SENSOR_IDLE;

        p_inst->val_diff = false;
        p_inst->is_ready = false;
        p_inst->is_init = true;

        // Exit here if initialization is successful
        status = CURRENT_SENSOR_OK;
    }
    else
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    return status;
}

current_sensor_status_t DEV_Current_Sensor_Task(dev_current_sensor_t *p_inst)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    adc_state_t adc_state;
    adc_status_t adc_status;
    bool adc_ready;

    time_status_t time_status;
    uint32_t now_time;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->adc_inst != NULL) && (p_inst->time_inst != NULL))
    {
        if(p_inst->is_init)
        {

            if(IO_ADC_Task(p_inst->adc_inst) != ADC_STATUS_OK)
            {
                status = CURRENT_SENSOR_ADC_ERROR;

                p_inst->state = CURRENT_SENSOR_ERROR;
            }

            if(p_inst->state >= CURRENT_SENSOR_STATE_MAX)
            {
                p_inst->state = CURRENT_SENSOR_STATE_UNDEF;
            }

            switch(p_inst->state)
            {
                case CURRENT_SENSOR_IDLE:
                {

                    if(p_inst->start_time != 0)
                    {
                        time_status = UTIL_Time_Get_Tick(p_inst->time_inst, now_time);

                        if(time_status == TIME_STATUS_OK)
                        {
                            if(p_inst->current_timeout)
                            {
                                if(now_time - p_inst->start_time >= p_inst->current_timeout)
                                {
                                    status = CURRENT_SENSOR_TIMEOUT;

                                    p_inst->state = CURRENT_SENSOR_ERROR;
                                }
                            }
                        }
                        else
                        {
                            status = CURRENT_SENSOR_TIME_ERROR;

                            p_inst->state = CURRENT_SENSOR_ERROR;
                        }
                    }

                    break;
                }

                case CURRENT_SENSOR_START:
                {
                    p_inst->is_ready = false;

                    adc_status = IO_ADC_Get_State(p_inst->adc_inst, &adc_state);

                    if(adc_status == ADC_STATUS_OK)
                    {
                        if(adc_state == ADC_STATE_IDLE)
                        {
                            // Start ADC and switch states if no error
                            if(IO_ADC_Start(p_inst->adc_inst) == ADC_STATUS_OK)
                            {
                                time_status = UTIL_Time_Get_Tick(p_inst->time_inst, p_inst->start_time);

                                if(time_status == TIME_STATUS_OK)
                                {
                                    p_inst->state = CURRENT_SENSOR_WAIT;
                                }
                                else
                                {
                                    status = CURRENT_SENSOR_TIME_ERROR;

                                    p_inst->state = CURRENT_SENSOR_ERROR;
                                }
                            }
                            else 
                            {
                                status = CURRENT_SENSOR_ADC_ERROR;

                                p_inst->state = CURRENT_SENSOR_ERROR;
                            }
                        }
                        else 
                        {
                            status = CURRENT_SENSOR_STATE_MISMATCH;

                            p_inst->state = CURRENT_SENSOR_ERROR;
                        }
                    }
                    else 
                    {
                        status = CURRENT_SENSOR_ADC_ERROR;

                        p_inst->state = CURRENT_SENSOR_ERROR;
                    }

                    break;
                }

                case CURRENT_SENSOR_WAIT:
                {
                    time_status = UTIL_Time_Get_Tick(p_inst->time_inst, now_time);

                    if(time_status == TIME_STATUS_OK)
                    {
                        // Check if time elapsed is less than the ADC timeout 
                        if(now_time - p_inst->start_time <= p_inst->adc_timeout_ms)
                        {
                            adc_status = IO_ADC_Get_Ready_Flag(p_inst->adc_inst, &adc_ready);

                            if(adc_status == ADC_STATUS_OK)
                            {
                                // Switches state if ADC returns ready
                                if(adc_ready)
                                {
                                    p_inst->state = CURRENT_SENSOR_GET;
                                }
                            }
                            else 
                            {
                                status = CURRENT_SENSOR_ADC_ERROR;

                                p_inst->state = CURRENT_SENSOR_ERROR;
                            }
                        }
                        else 
                        {
                            status = CURRENT_SENSOR_ADC_ERROR;

                            p_inst->state = CURRENT_SENSOR_ERROR;
                        }
                    }
                    else
                    {
                        status = CURRENT_SENSOR_TIME_ERROR;

                        p_inst->state = CURRENT_SENSOR_ERROR;
                    }

                    break;
                }

                case CURRENT_SENSOR_GET:
                {
                    adc_status = IO_ADC_Get_Ready_Flag(p_inst->adc_inst, &adc_ready);

                    if(adc_ready)
                    {
                        status = DEV_Current_Sensor_Process_Raw(p_inst);

                        if(status == CURRENT_SENSOR_OK)
                        {
                            p_inst->state = CURRENT_SENSOR_READY;
                        }
                        else 
                        {
                            p_inst->state = CURRENT_SENSOR_ERROR;
                        }
                    }
                    else 
                    {
                        // ADC timing error has occured since ADC has to return ready to switch into this state
                        status = CURRENT_SENSOR_ADC_ERROR;

                        p_inst->state = CURRENT_SENSOR_ERROR;
                    }

                    break;
                }

                case CURRENT_SENSOR_READY:
                {
                    // Starts timer and switches state to IDLE
                    p_inst->is_ready = true;

                    time_status = UTIL_Time_Get_Tick(p_inst->time_inst, p_inst->start_time);

                    if(time_status == TIME_STATUS_OK)
                    {
                        p_inst->state = CURRENT_SENSOR_IDLE;
                    }
                    else
                    {
                        status = CURRENT_SENSOR_TIME_ERROR;

                        p_inst->state = CURRENT_SENSOR_ERROR;
                    }

                    break;
                }

                case CURRENT_SENSOR_ERROR:
                {
                    // Returns error state
                    break;
                }

                // Max enum for states required by standards
                case CURRENT_SENSOR_STATE_UNDEF:
                {
                    status = CURRENT_SENSOR_UNDEF_STATE;

                    p_inst->state = CURRENT_SENSOR_ERROR;

                   break;
                }

                default:
                {
                    /*no action required*/

                    break;
                }
            }
        }
        else 
        {
            status = CURRENT_SENSOR_NOT_INIT;
        }
    }
    else
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    return status;
}

current_sensor_status_t DEV_Current_Sensor_Start(dev_current_sensor_t *p_inst)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    current_sensor_state_t state;

    if(p_inst != NULL)
    {
        if(p_inst->is_init)
        {
            // Checks if state is IDLE
            status = DEV_Current_Sensor_Get_State( p_inst, &state);

            if(status == CURRENT_SENSOR_OK)
            {
                if(state == CURRENT_SENSOR_IDLE)
                {
                    p_inst->state = CURRENT_SENSOR_START;
                }
                else 
                {
                    status = CURRENT_SENSOR_BUSY;
                }
            }
        }
        else 
        {
            status = CURRENT_SENSOR_NOT_INIT;
        }
    }
    else 
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    return status;
}


current_sensor_status_t DEV_Current_Sensor_Process_Raw(dev_current_sensor_t *p_inst)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    adc_status_t adc_status;

    int32_t current_mA = 0; // initialize at safe value;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->adc_inst != NULL))
    {
        if(p_inst->is_init)
        {
            uint16_t raw = 0U;

            uint16_t vref = 0U;

            uint16_t resolution = 0U;

            int16_t offset = 0;

            int32_t gain_uV = 0;

            adc_status = IO_ADC_Get_Val(p_inst->adc_inst, &raw);

            if(adc_status != ADC_STATUS_OK)
            {
                status = CURRENT_SENSOR_ADC_ERROR;
            }

            adc_status = IO_ADC_Get_Vref(p_inst->adc_inst, &vref);

            if(adc_status != ADC_STATUS_OK)
            {
                status = CURRENT_SENSOR_ADC_ERROR;
            }

            adc_status = IO_ADC_Get_Resolution(p_inst->adc_inst, &resolution);

            if(adc_status != ADC_STATUS_OK)
            {
                status = CURRENT_SENSOR_ADC_ERROR;
            }

            adc_status = IO_ADC_Get_Offset(p_inst->adc_inst, &offset);
            
            if(adc_status != ADC_STATUS_OK)
            {
                status = CURRENT_SENSOR_ADC_ERROR;
            }

            status = DEV_Current_Sensor_Get_Gain(p_inst, &gain_uV);

            if((resolution > 0U) && (gain_uV != 0))
            {
                uint32_t voltage_mV = (raw * vref) / resolution;

                int32_t delta_mV = (int32_t)voltage_mV - (int32_t)offset;

                current_mA = (delta_mV * MV_TO_UV) / gain_uV;
            }

            if(current_mA != p_inst->last_val)
            {
                p_inst->val_diff = true;
            }

            p_inst->last_val = current_mA;
        }
    }
    else 
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    return status;
}

current_sensor_status_t DEV_Current_Sensor_Set_Timeout(dev_current_sensor_t *p_inst, uint32_t new_val)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    if(p_inst != NULL)
    {
        if(p_inst->is_init)
        {
            if(new_val) // checks if delay between samples is valid (not 0)
            {
                p_inst->current_timeout = new_val;
            }
        }
        else 
        {
            status = CURRENT_SENSOR_NOT_INIT;
        }
    }
    else 
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    return status;
}

current_sensor_status_t DEV_Current_Sensor_Get_Wait(dev_current_sensor_t *p_inst, uint32_t *p_out)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    uint32_t wait = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            wait = p_inst->current_timeout;
        }
        else
        {
            status = CURRENT_SENSOR_NOT_INIT;
        }
    }
    else 
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    *p_out = wait;

    return status;
}

current_sensor_status_t DEV_Current_Sensor_Get_Gain(dev_current_sensor_t *p_inst, int32_t *p_out)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    int32_t gain = 0;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            gain = p_inst->cfg->current_gain_uV;
        }
        else 
        {
            status = CURRENT_SENSOR_NOT_INIT;
        }
    }
    else 
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    *p_out = gain;

    return status;
}

current_sensor_status_t DEV_Current_Sensor_Get_Val(dev_current_sensor_t *p_inst, int16_t *p_out)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    int32_t val = 0;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            val = p_inst->last_val;
        }
        else 
        {
            status = CURRENT_SENSOR_NOT_INIT;
        }
    }
    else
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    *p_out = val;

    return status;
}

current_sensor_status_t DEV_Current_Sensor_Get_State(dev_current_sensor_t *p_inst, current_sensor_state_t *p_out)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    current_sensor_state_t state = CURRENT_SENSOR_STATE_UNDEF;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            state = p_inst->state;
        }
        else 
        {
            status = CURRENT_SENSOR_NOT_INIT;
        }
    }
    else 
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    *p_out = state;

    return status;
}

current_sensor_status_t DEV_Current_Sensor_Get_Data_Diff(dev_current_sensor_t *p_inst, bool *p_out)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    bool diff_flag = false;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            diff_flag = p_inst->val_diff;
        }
        else 
        {
            status = CURRENT_SENSOR_NOT_INIT;
        }

        *p_out = diff_flag; 
    }
    else 
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    return status;
}

current_sensor_status_t DEV_Current_Sensor_Get_Ready_Flag(dev_current_sensor_t *p_inst, bool *p_out)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    bool flag = false;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            flag = p_inst->is_ready;
        }
        else
        {
            status = CURRENT_SENSOR_NOT_INIT;
        }

        *p_out = flag;
    }
    else
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    return status;
}

current_sensor_status_t DEV_Current_Sensor_Get_Timeout(dev_current_sensor_t *p_inst, uint32_t *p_out)
{
    current_sensor_status_t status = CURRENT_SENSOR_OK;

    uint32_t timeout = 0U;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            timeout = p_inst->current_timeout;
        }
        else
        {
            status = CURRENT_SENSOR_NOT_INIT;
        }

        *p_out = timeout;
    }
    else
    {
        status = CURRENT_SENSOR_NULL_POINTER;
    }

    return status;
}