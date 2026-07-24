/**
 * @file io_adc.c
 * @author notwe
 * @date 2026-05-03
 * @brief abstract adc i/o source
 */

#include "io_adc.h"

adc_status_t IO_ADC_Init(adc_t *p_inst, const adc_cfg_t *p_cfg)
{
    adc_status_t status = ADC_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL))
    {
        if(p_inst->is_init)
        {
            if(&p_cfg->adc_func_cfg != NULL)
            {
                if(!((&p_cfg->adc_func_cfg.ADC_channel_select != NULL) &&
                    (&p_cfg->adc_func_cfg.ADC_start != NULL) &&
                    (&p_cfg->adc_func_cfg.ADC_done != NULL) &&
                    (&p_cfg->adc_func_cfg.ADC_get_result != NULL) &&
                    (&p_cfg->adc_func_cfg.ADC_stop != NULL)))
                {
                    status = ADC_NULL_FUNC;
                }

                if(status != ADC_NULL_FUNC)
                {
                    p_inst->adc_ctx = p_cfg->ctx_cfg;
                    p_inst->adc_func = p_cfg->adc_func_cfg;
                    p_inst->p_time_inst = p_cfg->time_cfg;

                    p_inst->start_time = 0U;

                    p_inst->set_offset = p_inst->adc_ctx.adc_offset;

                    p_inst->state = ADC_STATE_IDLE;

                    p_inst->is_ready = false;

                    p_inst->is_init = true;

                    status = ADC_OK;
                }
            }
            else 
            {
                status = ADC_NULL_PTR;
            }
        }
        else
        {
            status = ADC_DBL_INIT;
        }
    }
    else
    {
        status = ADC_NULL_PTR;
    }

    return status;
}

adc_status_t IO_ADC_Task(adc_t *p_inst)
{
    adc_status_t status = ADC_OK;
    time_status_t time_status;
    uint32_t now_time = 0U;

    if((p_inst != NULL))
    {
        if(p_inst->is_init)
        {
            if((p_inst->state >= ADC_STATE_MAX) || (p_inst->state == ADC_STATE_UNDEF))
            {
                status = ADC_UNDEF_STATE;

                p_inst->state = ADC_STATE_ERROR;
            }

            switch(p_inst->state)
            {
                case ADC_STATE_IDLE:
                {
                    time_status = UTIL_Time_Get_Tick(p_inst->p_time_inst, &now_time);

                    if(time_status == TIME_STATUS_OK)
                    {
                        if(p_inst->start_time)
                        {
                            if(now_time - p_inst->start_time > p_inst->adc_ctx.adc_timeout)
                            {
                                status = ADC_TIMEOUT;
                            }
                        }
                    }
                    else
                    {
                        status = ADC_TIME_FAULT;

                        p_inst->state = ADC_STATE_ERROR;
                    }

                    break;
                }

                case ADC_STATE_START:
                {
                    // select adc channel (defined in instance configuration in main.c)
                    p_inst->adc_func.ADC_channel_select(p_inst->adc_ctx.channel_id);

                    // start adc measurement on selected channel
                    p_inst->adc_func.ADC_start();

                    p_inst->is_ready = false;

                    time_status = UTIL_Time_Get_Tick(p_inst->p_time_inst, &p_inst->start_time);

                    if(time_status == TIME_STATUS_OK)
                    {
                        p_inst->state = ADC_STATE_WAIT;
                    }
                    else
                    {
                        status = ADC_TIME_FAULT;

                        p_inst->state = ADC_STATE_ERROR;
                    }

                    break;
                }

                case ADC_STATE_WAIT:
                {
                    time_status = UTIL_Time_Get_Tick(p_inst->p_time_inst, &now_time);
                    
                    if(time_status == TIME_STATUS_OK)
                    {
                        if(now_time - p_inst->start_time <= p_inst->adc_ctx.adc_timeout)
                        {
                            if(!p_inst->adc_ctx.adc_wait_ms)
                            {
                                if(p_inst->adc_func.ADC_done()) 
                                {
                                    p_inst->start_time = now_time;

                                    p_inst->state = ADC_STATE_GET;
                                }
                            }
                            else if(now_time - p_inst->start_time < p_inst->adc_ctx.adc_wait_ms)
                            {
                                p_inst->start_time = now_time;

                                p_inst->state = ADC_STATE_GET;
                            }
                            else
                            {
                                status = ADC_BUSY;
                            }
                        }
                        else
                        {
                            status = ADC_TIMEOUT;

                            p_inst->state = ADC_STATE_ERROR;
                        }
                    }
                    else
                    {
                        status = ADC_TIME_FAULT;

                        p_inst->state = ADC_STATE_ERROR;
                    }

                    break;
                }

                case ADC_STATE_GET:
                {
                    time_status = UTIL_Time_Get_Tick(p_inst->p_time_inst, &now_time);

                    if(time_status == TIME_STATUS_OK)
                    {
                        if(now_time - p_inst->start_time > p_inst->adc_ctx.adc_timeout)
                        {
                            status = ADC_TIMEOUT;

                            p_inst->state = ADC_STATE_ERROR;
                        }
                        else
                        {
                            // stores measurement
                            p_inst->last_raw = p_inst->adc_func.ADC_get_result();

                            p_inst->start_time = now_time;

                            p_inst->state = ADC_STATE_READY;
                        }
                    }
                    else
                    {
                        status = ADC_TIME_FAULT;

                        p_inst->state = ADC_STATE_ERROR;
                    }

                    break;
                }

                case ADC_STATE_READY:
                {
                    time_status = UTIL_Time_Get_Tick(p_inst->p_time_inst, &now_time);

                    if(time_status == TIME_STATUS_OK)
                    {
                        if(now_time - p_inst->start_time > p_inst->adc_ctx.adc_timeout)
                        {
                            status = ADC_TIMEOUT;

                            p_inst->state = ADC_STATE_ERROR;
                        }
                        else
                        {
                            p_inst->is_ready = true;

                            p_inst->start_time = now_time;

                            p_inst->state = ADC_STATE_IDLE;
                        }
                    }
                    else
                    {
                        status = ADC_TIME_FAULT;

                        p_inst->state = ADC_STATE_ERROR;
                    }

                    break;
                }

                case ADC_STATE_ERROR:
                {
                    // indicates the adc has encountered an error, intentionally left blank
                    break;
                }

                default:
                {
                    p_inst->state = ADC_STATE_ERROR;

                    break;
                }
    
            }
        }
        else
        {
            status = ADC_NOT_INIT;
        }

    }
    else 
    {
        status = ADC_NULL_PTR;
    }

    return status;

}

adc_status_t IO_ADC_Start(adc_t *p_inst)
{
    adc_status_t status = ADC_OK;

    if((p_inst != NULL))
    {
        if(p_inst->is_init)
        {
            if(p_inst->state == ADC_STATE_IDLE)
            {
                p_inst->state = ADC_STATE_START;
            }
            else
            {
                status = ADC_BUSY;
            }
        }
        else 
        {
            status = ADC_NOT_INIT;
        }
    }
    else 
    {
        status = ADC_NULL_PTR;
    }

    return status;
}

adc_status_t IO_ADC_Get_Val(adc_t *p_inst, uint16_t *p_out)
{
    adc_status_t status = ADC_OK;

    uint32_t val = 0U;

    if((p_inst != NULL) && (p_out != NULL)) 
    {
        if(p_inst->is_init)
        {
            // stores adc measurement in temp value accessible by external functions
            val = p_inst->last_raw;
        }
        else 
        {
            status = ADC_NOT_INIT;
        }

        *p_out = val;
    }
    else
    {
        status = ADC_NULL_PTR;
    }

    return status;
}

adc_status_t IO_ADC_Get_Resolution(adc_t *p_inst, uint16_t *p_out)
{
    adc_status_t status = ADC_OK;

    uint16_t resolution = 0U;

    if((p_inst != NULL) && (p_out != NULL)) 
    {
        if(p_inst->is_init)
        {
            resolution = p_inst->adc_ctx.adc_resolution;
        }
        else 
        {
            status = ADC_NOT_INIT;
        }

        *p_out = resolution;
    }
    else
    {
        status = ADC_NULL_PTR;
    }

    return status;
}

adc_status_t IO_ADC_Get_Offset(adc_t *p_inst, int16_t *p_out)
{
    adc_status_t status = ADC_OK;

    int16_t offset = 0;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            offset = p_inst->set_offset;
        }
        else 
        {
            status = ADC_NOT_INIT;
        }

        *p_out = offset;
    } 
    else
    {
        status = ADC_NULL_PTR;
    }

    return status;  
}

adc_status_t IO_ADC_Set_Offset(adc_t *p_inst, int16_t new_val)
{
    adc_status_t status = ADC_OK;

    if(p_inst != NULL)
    {
        if(p_inst->is_init)
        {
            p_inst->set_offset = new_val;
        }
        else 
        {
            status = ADC_NOT_INIT;
        }
    }
    else 
    {
        status = ADC_NULL_PTR;
    }

    return status;
}

adc_status_t IO_ADC_Get_Vref(adc_t *p_inst, uint16_t *p_out)
{
    adc_status_t status = ADC_OK;

    uint16_t vref = 0U;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            vref = p_inst->adc_ctx.adc_vref_mV;
        }
        else 
        {
            status = ADC_NOT_INIT;
        }

        *p_out = vref;
    }
    else
    {
        status = ADC_NULL_PTR;
    }

    return status;
}

adc_status_t IO_ADC_Get_State(adc_t *p_inst, adc_state_t *p_out)
{
    adc_status_t status = ADC_OK;

    adc_state_t state;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            state = p_inst->state;
        }
        else 
        {
            status = ADC_NOT_INIT;
        }

        *p_out = state;
    }
    else 
    {
        status = ADC_NULL_PTR;
    }

    return status;
}

adc_status_t IO_ADC_Get_Timeout(adc_t *p_inst, uint32_t *p_out)
{
    adc_status_t status = ADC_OK;

    uint32_t timeout = 0U;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            timeout = p_inst->adc_ctx.adc_timeout;
        }
        else 
        {
            status = ADC_NOT_INIT;
        }

        *p_out = timeout;
    }
    else 
    {
        status = ADC_NULL_PTR;
    }

    return status;
}

adc_status_t IO_ADC_Get_Ready_Flag(adc_t *p_inst, bool *p_out)
{
    adc_status_t status = ADC_OK;

    bool flag = false;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            flag = p_inst->is_ready;
        }
        else
        {
            status = ADC_NOT_INIT;
        }
    }
    else
    {
        status = ADC_NULL_PTR;
    }

    *p_out = flag;

    return status;
}
