/**
 * @file DEV_current.h
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor device driver source
 */

#include "dev_current.h"

current_status_t DEV_Current_Init(dev_current_t *p_inst, const dev_current_cfg_t *p_cfg, const io_adc_t *p_adc_inst)
{
    current_status_t status = CURRENT_STATUS_ERROR_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL) && (p_adc_inst != NULL))
    {
        p_inst->cfg = p_cfg;
        p_inst->adc_inst = p_adc_inst;

        p_inst->cal_acc = 0U;
        p_inst->cal_count = 0U;
        p_inst->raw_offset = 0U;

        p_inst->start_time = UTIL_Time_Get_Tick();

        p_inst->adc_timeout_ms = p_inst->adc_inst->cfg->adc_timeout;

        p_inst->is_init = true;

        status = CURRENT_STATUS_OK;
    }
    else
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

current_status_t DEV_Current_Task(dev_current_t *p_inst)
{
    current_status_t status = CURRENT_STATUS_OK;

    adc_state_t adc_state;
    adc_status_t adc_status;

    uint32_t now_time;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->adc_inst != NULL))
    {
        if(p_inst->is_init)
        {
            switch(p_inst->state)
            {
                case CURRENT_STATE_IDLE:
                {
                    now_time = UTIL_Time_Get_Tick();

                    if(now_time - p_inst->start_time > p_inst->cfg->current_wait_ms)
                    {
                        status = ADC_STATUS_ERROR_TIMEOUT;

                        p_inst->state = CURRENT_STATE_ERROR;
                    }

                    break;
                }

                case CURRENT_STATE_START:
                {
                    adc_status = IO_ADC_Get_State(p_inst->adc_inst, &adc_state);

                    if(adc_status == ADC_STATUS_OK)
                    {
                        if(adc_state == ADC_STATE_IDLE)
                        {
                            if(IO_ADC_Start(p_inst->adc_inst) == ADC_STATUS_OK)
                            {
                                p_inst->start_time = UTIL_Time_Get_Tick();

                                p_inst->state = CURRENT_STATE_WAIT;
                            }
                            else 
                            {
                                status = CURRENT_STATUS_ERROR_ADC_ERROR;

                                p_inst->state = CURRENT_STATE_ERROR;
                            }
                        }
                        else 
                        {
                            status = CURRENT_STATUS_ERROR_RACE;

                            p_inst->state = CURRENT_STATE_ERROR;
                        }
                    }
                    else 
                    {
                        status = CURRENT_STATUS_ERROR_ADC_ERROR;

                        p_inst->state = CURRENT_STATE_ERROR;
                    }

                    break;
                }

                case CURRENT_STATE_WAIT:
                {
                    now_time = UTIL_Time_Get_Tick();

                    if(now_time - p_inst->start_time <= p_inst->adc_timeout_ms)
                    {
                        adc_status = IO_ADC_Get_State(p_inst->adc_inst, &adc_state);

                        if(adc_status == ADC_STATUS_OK)
                        {
                            if(adc_state != ADC_STATE_READY)
                            {
                                if(IO_ADC_Task(p_inst->adc_inst) != ADC_STATUS_OK)
                                {
                                    status = CURRENT_STATUS_ERROR_ADC_ERROR;

                                    p_inst->state = CURRENT_STATE_ERROR;
                                }
                            }
                            else 
                            {
                                p_inst->state = CURRENT_STATE_GET;
                            }
                        }
                        else 
                        {
                            status = CURRENT_STATUS_ERROR_ADC_ERROR;

                            p_inst->state = CURRENT_STATE_ERROR;
                        }
                    }
                    else 
                    {
                        status = CURRENT_STATUS_ERROR_ADC_ERROR;

                        p_inst->state = CURRENT_STATE_ERROR;
                    }

                    break;
                }

                case CURRENT_STATE_GET:
                {
                    adc_status = IO_ADC_Get_State(p_inst->adc_inst, &adc_state);

                    if(adc_state == ADC_STATE_READY)
                    {
                        status = DEV_Current_Process_Raw(p_inst);

                        if(status == CURRENT_STATUS_OK)
                        {
                            p_inst->state = CURRENT_STATE_READY;
                        }
                        else 
                        {
                            p_inst->state = CURRENT_STATE_ERROR;
                        }
                    }
                    else 
                    {
                        status = CURRENT_STATUS_ERROR_ADC_ERROR;

                        p_inst->state = CURRENT_STATE_ERROR;
                    }

                    break;
                }

                case CURRENT_STATE_READY:
                {
                    p_inst->start_time = UTIL_Time_Get_Tick();

                    p_inst->state = CURRENT_STATE_IDLE;

                    break;
                }

                case CURRENT_STATE_ERROR:
                {
                    
                    break;
                }

                case CURRENT_STATE_UNDEFINED:
                {
                    status = CURRENT_STATUS_ERROR_UNDEFINED_STATE;

                    p_inst->state = CURRENT_STATE_ERROR;

                   break;
                }
            }
        }
        else 
        {
            status = CURRENT_STATUS_ERROR_NOT_INIT;
        }
    }
    else
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

current_status_t DEV_Current_Process_Raw(dev_current_t *p_inst)
{
    current_status_t status = CURRENT_STATUS_OK;

    adc_status_t adc_status;

    int32_t current_mA = 0; // initialize at safe value;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->adc_inst != NULL))
    {
        if(p_inst->is_init)
        {
            uint32_t raw = 0U;

            uint16_t vref = 0U;

            uint16_t resolution = 0U;

            int16_t offset = 0;

            int32_t gain_uV = 0;

            status = DEV_Current_Get_Raw(p_inst, &raw);

            adc_status = IO_ADC_Get_Vref(p_inst->adc_inst, &vref);

            if(adc_status != ADC_STATUS_OK)
            {
                status = CURRENT_STATUS_ERROR_ADC_ERROR;
            }

            adc_status = IO_ADC_Get_Resolution(p_inst->adc_inst, &resolution);

            if(adc_status != ADC_STATUS_OK)
            {
                status = CURRENT_STATUS_ERROR_ADC_ERROR;
            }

            adc_status = IO_ADC_Get_Offset(p_inst->adc_inst, &offset);
            
            if(adc_status != ADC_STATUS_OK)
            {
                status = CURRENT_STATUS_ERROR_ADC_ERROR;
            }

            status = DEV_Current_Get_Gain(p_inst, &gain_uV);

            if((resolution > 0U) && (gain_uV != 0))
            {
                uint32_t voltage_mV = (raw * vref) / resolution;

                int32_t delta_mV = (int32_t)voltage_mV - (int32_t)offset;

                current_mA = (delta_mV * MV_TO_UV) / gain_uV;
            }

            p_inst->last_val = current_mA;
        }
    }
    else 
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

current_status_t DEV_Current_Get_Wait(dev_current_t *p_inst, uint32_t *p_out)
{
    current_status_t status = CURRENT_STATUS_OK;

    uint32_t wait = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            wait = p_inst->cfg->current_wait_ms;
        }
        else
        {
            status = CURRENT_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;
    }

    *p_out = wait;

    return status;
}

current_status_t DEV_Current_Get_Gain(dev_current_t *p_inst, int32_t *p_out)
{
    current_status_t status = CURRENT_STATUS_OK;

    int32_t gain = 0;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            gain = p_inst->cfg->current_gain_uV;
        }
        else 
        {
            status = CURRENT_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;
    }

    *p_out = gain;

    return status;
}

current_status_t DEV_Current_Get_Raw(dev_current_t *p_inst, uint32_t *p_out)
{
    current_status_t status = CURRENT_STATUS_OK;

    adc_status_t adc_status;

    int64_t raw = 0; // large unsigned integer (long long) so as to not lose accuracy

    if((p_inst != NULL) && (p_inst->adc_inst != NULL))
    {
        if(p_inst->is_init)
        {
            adc_status = (int64_t)IO_ADC_Get_Val(p_inst->adc_inst, &raw);

            if(adc_status != ADC_STATUS_OK)
            {
                status = CURRENT_STATUS_ERROR_ADC_ERROR;
            }

            raw -= (int64_t)p_inst->raw_offset; 

            if(raw < 0)
            {
                raw = 0; // value clamping;
            }
        }
        else 
        {
            status = CURRENT_STATUS_ERROR_NOT_INIT;
        }
    }
    else
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;
    }

    *p_out = (uint32_t)raw;

    return status;
}

current_status_t DEV_Current_Get_Val(dev_current_t *p_inst, int32_t *p_out)
{
    current_status_t status = CURRENT_STATUS_OK;

    int32_t val = 0;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            val = p_inst->last_val;
        }
        else 
        {
            status = CURRENT_STATUS_ERROR_NOT_INIT;
        }
    }
    else
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;
    }

    *p_out = val;

    return status;
}

current_status_t DEV_Current_Get_State(dev_current_t *p_inst, current_state_t *p_out)
{
    current_status_t status = CURRENT_STATUS_OK;

    current_state_t state = CURRENT_STATE_UNDEFINED;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            state = p_inst->state;
        }
        else 
        {
            status = CURRENT_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;
    }

    *p_out = state;

    return status;
}


